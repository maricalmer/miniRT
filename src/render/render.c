#include "minirt.h"

void	average_hd_pixel(int (*hd_res)[3], int hd_size);
// void	calculate_ray_prim_dir(t_data *data);
// void	calculate_ray_prim_dir_packet(void *arg_generic);
void	iterate_hd_pxl(float *primary_rays, float dx, float top_left_x, float top_left_y, int anti_fa);
void 	calculate_hit_pt(float t, t_shoot *shoot);
void 	get_normal_intersect(t_shoot *shoot);
void 	init_data_ray(t_ray_prim *data_ray, t_data *data);

void	render_first_image(t_data *data)
{
	clock_t start;
	t_bvh 	*bvh;
	

	start = clock();
	if (BVH_ON)
	{
		bvh = init_bvh(data); // rather return NULL  is no elements
		if (bvh->childs[0] != -2)
		{
			update_group(data, bvh);
			data->is_bvh = 1;
		}
		else
		{
			data->is_bvh = 0;
			free(bvh);
		}
	}
	// start of init_2
	data->cam.world_center[0] = (bvh->min_x[0] + bvh->max_x[0]) * 0.5;
	data->cam.world_center[1] = (bvh->min_y[0] + bvh->max_y[0]) * 0.5; 
	data->cam.world_center[2] = (bvh->min_z[0] + bvh->max_z[0]) * 0.5;
	cpy_vec(data->cam.origin, data->cam.origin_backup);
	data->mouse_pressed_l = 0;
	data->mouse_pressed_r = 0;
	// end of init_2

	first_rotation_matrice(data);
	printf("%s    > %.2f ms to build BVH\n", CYAN_TXT_START, ((double)(clock() - start)) / CLOCKS_PER_SEC * 1000);
	printf("\n  [RENDER]%s\n", COLOR_END);
	calculate_img(data);
}

void 	calculate_img(t_data *data)
{
	int				i;
	t_ray_prim 		data_ray;
	static int		n_img;
	struct timeval	t_start, t_end;			

	gettimeofday(&t_start, NULL);
	init_data_ray(&data_ray, data);
	pthread_mutex_lock(&data->joblist_mutex);
	data->joblist_top = 0;
	i = -1;
	while (++i < HEIGHT)
	{
		data->joblist[i].x_tl = -data_ray.max_x;
		data->joblist[i].y_tl = data_ray.max_y - data_ray.dx * i;
		data->joblist[i].dx = data_ray.dx;
		data->joblist[i].dx_hd = data_ray.hd_dx;
		data->joblist[i].data = data;
		data->joblist[i].line = i;
	}
	pthread_mutex_unlock(&data->joblist_mutex);
	wait_for_workers(data);
	mlx_put_image_to_window(data->mlx.mlx, data->mlx.win, data->mlx.img, 0, 0);
	mlx_do_sync(data->mlx.mlx);
	gettimeofday(&t_end, NULL);
	float t = (t_end.tv_sec - t_start.tv_sec) * 1000 + (t_end.tv_usec - t_start.tv_usec) * 0.001; 
	printf("%s    > img %d rendered after %.1f ms%s\n", CYAN_TXT_START, n_img++, t, COLOR_END);
}


void calc_p_ray(float x, float y, float res[3], float r_mat[3][3])
{
	res[0] = x;
	res[1] = y;
	res[2] = -1;
	normalize(res);
	dot_inplace_33_13(r_mat, res);
}



void 	calculate_img_packet(t_calc_img_arg *arg)
{
	t_shoot 		first_shoot;
	int				i;
	int				j;
	int				k;
	int				l;

	float			x;
	float			y;
	int				(*hd_res)[3]; 

	hd_res = malloc(sizeof(int[3]) * arg->data->antialiasing_fact * arg->data->antialiasing_fact);
	first_shoot.src = arg->data->cam.origin;
	first_shoot.depth = 0;
	first_shoot.inside = 0;
	i = -1;
	while (++i < WIDTH) //pixel
	{
		y = arg->y_tl - arg->dx_hd;
		j = -1;
		while (++j < arg->data->antialiasing_fact) // hd line
		{
			x = arg->x_tl + arg->dx * i + arg->dx_hd;
			k = -1;
			while (++k < arg->data->antialiasing_fact) // hd_colum
			{
				calc_p_ray(x, y, first_shoot.dir, arg->data->cam.r_mat);
				shoot_ray(arg->data, &first_shoot);
				x += arg->dx_hd;
				l = -1;
				while (++l< 3)
					hd_res[j * arg->data->antialiasing_fact + k][l] = imin(first_shoot.res_rgb[l], 255);
			}
			y -= arg->dx_hd;
		}
		average_hd_pixel(hd_res, arg->data->antialiasing_fact * arg->data->antialiasing_fact);
		arg->data->mlx.addr[i + arg->line * WIDTH] = (hd_res[0][0] << 16 | hd_res[0][1] << 8 | hd_res[0][2]);
	}
	free(hd_res);
}

void init_data_ray(t_ray_prim *data_ray, t_data *data)
{
	data_ray->max_x = tan(data->cam.fov * M_PI / 360);
	data_ray->max_y = data_ray->max_x * HEIGHT / WIDTH;
	data_ray->size_hd = data->antialiasing_fact * data->antialiasing_fact * 3;
	data_ray->dx = 2 * data_ray->max_x / WIDTH;
	data_ray->hd_dx = data_ray->dx / data->antialiasing_fact;
}

void	average_hd_pixel(int (*hd_res)[3], int hd_size)
{
	int		i;
	int		j;
	float	divisor;

	i = 0;
	while (++i < hd_size)
	{
		j = -1;
		while (++j < 3)
			hd_res[0][j] += hd_res[i][j];
	}
	j = -1;
	divisor = (float)1 / (hd_size);
	while (++j < 3)
		hd_res[0][j] *= divisor;
}

void	shoot_ray(t_data *data, t_shoot *shoot)
{
	float t;

	t = visibility_intersection_tests(data->objects, shoot, data->n_obj);
	calculate_hit_pt(t, shoot);
	get_normal_intersect(shoot);
	shading(shoot, data);
}

void calculate_hit_pt(float t, t_shoot *shoot)
{
	if (t > EPSILON)
	{
		if (shoot->obj->type == TRI)
		{
			float   px[3][3];
			float   bary[3];
			float   denom;
			int     i;
		
			vec_substr(shoot->obj->geo.tri.v0, shoot->src, px[0]);
			vec_substr(shoot->obj->geo.tri.v1, shoot->src, px[1]);
			vec_substr(shoot->obj->geo.tri.v2, shoot->src, px[2]);
			bary[0] = triple_scalar(shoot->dir, px[2], px[1]);
			bary[1] = triple_scalar(shoot->dir, px[0], px[2]);
			bary[2] = triple_scalar(shoot->dir, px[1], px[0]);
			denom = 1.0 / (bary[0] + bary[1] + bary[2]);
			i = -1;
			while (++i < 3)
				bary[i] *= denom;
			i = -1;
			while (++i < 3)
				shoot->hit_pt[i] = bary[0] * shoot->obj->geo.tri.v0[i] + bary[1] * shoot->obj->geo.tri.v1[i] + bary[2] * shoot->obj->geo.tri.v2[i];
		}
		else 
		{
		shoot->hit_pt[0] = shoot->src[0] + t * shoot->dir[0];
		shoot->hit_pt[1] = shoot->src[1] + t * shoot->dir[1];
		shoot->hit_pt[2] = shoot->src[2] + t * shoot->dir[2];
		}
	}
	else
		shoot->obj = NULL;
}

void get_normal_intersect(t_shoot *shoot)
{
	float r_inv;
	
	if (!shoot->obj)
		return ;
	if (shoot->obj->type == SPHERE)
	{
		r_inv = 1 / shoot->obj->geo.sph.radius;
		shoot->normal[0] = r_inv * (shoot->hit_pt[0] - shoot->obj->geo.sph.center[0]);
		shoot->normal[1] = r_inv * (shoot->hit_pt[1] - shoot->obj->geo.sph.center[1]);
		shoot->normal[2] = r_inv * (shoot->hit_pt[2] - shoot->obj->geo.sph.center[2]);
	}
	else if (shoot->obj->type == PLANE)
	{
		shoot->normal[0] = shoot->obj->geo.pl.normal[0];
		shoot->normal[1] = shoot->obj->geo.pl.normal[1];
		shoot->normal[2] = shoot->obj->geo.pl.normal[2];
	}
	else if (shoot->obj->type == TRI)
	{
		int i;
		float   px[3][3];
		float   bary[3];

		vec_substr(shoot->obj->geo.tri.v0, shoot->src, px[0]);
		vec_substr(shoot->obj->geo.tri.v1, shoot->src, px[1]);
		vec_substr(shoot->obj->geo.tri.v2, shoot->src, px[2]);
		bary[0] = triple_scalar(shoot->dir, px[2], px[1]);
		bary[1] = triple_scalar(shoot->dir, px[0], px[2]);
		bary[2] = triple_scalar(shoot->dir, px[1], px[0]);
		float denom = 1.0 / (bary[0] + bary[1] + bary[2]);
		i = -1;
		while (++i < 3)
			bary[i] *= denom;
		i = -1;
		while (++i < 3)
		shoot->normal[i] = shoot->obj->geo.tri.n0[i] * bary[0]
						+ shoot->obj->geo.tri.n1[i] * bary[1]
						+ shoot->obj->geo.tri.n2[i] * bary[2];
	}
	else if (shoot->obj->type == CYLINDER)
	{
		int i = -1;
		float tmp[3];

		vec_substr(shoot->hit_pt, shoot->obj->geo.cyl.center, tmp);
		float h = dot_13_13(tmp, shoot->obj->geo.cyl.dir);
		while (++i < 3)
			tmp[i] = (shoot->obj->geo.cyl.center[i] + h) * shoot->obj->geo.cyl.dir[i];
		
		r_inv = 1 / shoot->obj->geo.cyl.radius;
		shoot->normal[0] = r_inv * (shoot->hit_pt[0] - tmp[0]);
		shoot->normal[1] = r_inv * (shoot->hit_pt[1] - tmp[1]);
		shoot->normal[2] = r_inv * (shoot->hit_pt[2] - tmp[2]);
	}

	// reverse if we hit inside !
	if (dot_13_13(shoot->dir, shoot->normal) > 0)
	{
		shoot->normal[0] *= -1;
		shoot->normal[1] *= -1;
		shoot->normal[2] *= -1;
	}
}
