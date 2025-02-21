#include "minirt.h"

void 	calculate_img(t_data *data, int *img);
void 	calculate_img_packet(void *arg_generic);
void	average_hd_pixel(int hd_res[ANTIALIASING_FACT * ANTIALIASING_FACT][3]);
void	calculate_ray_prim_dir(t_data *data);
void	calculate_ray_prim_dir_packet(void *arg_generic);
void	iterate_hd_pxl(float *primary_rays, float dx, float top_left_x, float top_left_y);
void 	calculate_hit_pt(float t, t_shoot *shoot);
void 	get_normal_intersect(t_shoot *shoot);
void	init_data_ray(t_ray_prim *data_ray, t_calc_img_arg *arg);

void	render_first_image(t_data *data, int *img) // img + mlx + win in data (??)
{
	clock_t start;
	
	start = clock();
	data->primary_rays = malloc(sizeof(float) * HEIGHT * WIDTH * 3 * ANTIALIASING_FACT * ANTIALIASING_FACT);
	printf("Elapsed time malloc prays: %.2f ms\n", ((double)(clock() - start)) / CLOCKS_PER_SEC * 1000);
	start = clock();
	calculate_ray_prim_dir(data);   // ===> apply multithreading (x N_THREADS)
	printf("Elapsed time P_rays: %.2f ms\n", ((double)(clock() - start)) / CLOCKS_PER_SEC * 1000);
	start = clock();
	if (BVH_ON)
	{
		t_aabb *root = init_bvh(data);
		update_group(data, root); //(only BVH and Planes)
	}
	printf("Elapsed time BVH tree: %.2f ms\n", ((double)(clock() - start)) / CLOCKS_PER_SEC * 1000);
	start = clock();
	calculate_img(data, img);
	printf("Elapsed time calc_imag: %.2f ms\n", ((double)(clock() - start)) / CLOCKS_PER_SEC * 1000);
}

// void	move_cam(t_data *data, int *img) // img + mlx + win in data (??)
// {
// 	double start = get_time();
// 	// move_cam() -> pt_origin
// 	// transform_p_ray()
// 	time_primary_rays += get_time() - start;
// 	(void) root;
// 	calculate_img(data, img);
// 	// put_img
// }


void 	calculate_img(t_data *data, int *img)
{
	int				i;
	t_calc_img_arg	*arg;
	int				packet_size;

	packet_size = WIDTH;
	arg = malloc(sizeof(t_calc_img_arg) * HEIGHT);
	pthread_mutex_lock(&data->joblist_mutex);
	data->joblist_top = 0;
	data->joblist_size = HEIGHT;
	i = -1;
	while (++i < HEIGHT)
	{
		arg[i].data = data;
		arg[i].img = img;
		arg[i].start = i * packet_size;
		arg[i].end = arg[i].start + packet_size;
		data->joblist[i].function = calculate_img_packet;
		data->joblist[i].arg = arg + i;
	}
	pthread_mutex_unlock(&data->joblist_mutex);
	wait_for_workers(data);
	free(arg);
	mlx_put_image_to_window(data->mlx.mlx, data->mlx.win, data->mlx.img, 0, 0);
}


void 	calculate_img_packet(void *arg_generic)
{
	t_shoot 		first_shoot;
	int				i;
	int				j;
	int				k;
	int	hd_res[ANTIALIASING_FACT * ANTIALIASING_FACT][3];
	t_calc_img_arg 	*arg;

	arg = arg_generic;

	first_shoot.src = arg->data->cam.origin;
	first_shoot.depth = 0;
	i = arg->start - 1;
	while (++i < arg->end)
	{
		j = -1;
		while (++j < ANTIALIASING_FACT * ANTIALIASING_FACT)
		{
			first_shoot.dir = &arg->data->primary_rays[i * 3 * ANTIALIASING_FACT * ANTIALIASING_FACT + j * 3];
			shoot_ray(arg->data, &first_shoot);
			k = -1;
			while (++k < 3)
				hd_res[j][k] = imin(first_shoot.res_rgb[k], 255);
		}
		average_hd_pixel(hd_res);
		arg->img[i] = (hd_res[0][0] << 16 | hd_res[0][1] << 8 | hd_res[0][2]);
	}
	// mlx_put_image_to_window(arg->data->mlx.mlx, arg->data->mlx.win, arg->data->mlx.img, 0, 0);
}

void	calculate_ray_prim_dir(t_data *data)  // duplicate of calculate_img: abstract to handler function at the cost of switch?
{
	int				i;
	t_calc_img_arg	*arg;
	int				packet_size;

	packet_size = WIDTH * HEIGHT / HEIGHT;
	arg = malloc(sizeof(t_calc_img_arg) * HEIGHT);
	pthread_mutex_lock(&data->joblist_mutex);
	data->joblist_top = 0;
	data->joblist_size = HEIGHT;
	i = -1;
	while (++i < HEIGHT)
	{
		arg[i].data = data;
		arg[i].start = i * packet_size;
		arg[i].end = arg[i].start + packet_size;
		data->joblist[i].function = calculate_ray_prim_dir_packet;
		data->joblist[i].arg = arg + i;
	}
	pthread_mutex_unlock(&data->joblist_mutex);
	wait_for_workers(data);
	free(arg);
}

void	calculate_ray_prim_dir_packet(void *arg_generic)
{
	t_ray_prim data_ray;
	int	i;
	int	j;
	int index;
	t_calc_img_arg 	*arg;

	arg = arg_generic;
	init_data_ray(&data_ray, arg);
	i = arg->start / WIDTH;
	data_ray.top_left_y = data_ray.max_y - (i * data_ray.dx);
	// while (i < data_ray.end_y)
	// {
		// j = data_ray.start_x;
		j = 0;
		data_ray.top_left_x = -data_ray.max_x; //+ (j * data_ray.dx);
		while (j < WIDTH)
		{
			index = i * WIDTH * data_ray.size_hd + j * data_ray.size_hd;
			iterate_hd_pxl(&arg->data->primary_rays[index], data_ray.hd_dx, data_ray.top_left_x, data_ray.top_left_y);
			j++;
			data_ray.top_left_x += data_ray.dx;

			// cam rotation missing here...
		}
		// i++;
		// data_ray.top_left_y -= data_ray.dx;
	// }
}

void init_data_ray(t_ray_prim *data_ray, t_calc_img_arg *arg)
{
	// ==> all of that could be calculated once only. But not a game changer anyway :)
	// data_ray->start_x = (arg->start % WIDTH);
	// data_ray->end_y = (arg->end / WIDTH);
	data_ray->max_x = tan(arg->data->cam.fov * M_PI / 360);
	data_ray->max_y = data_ray->max_x * HEIGHT / WIDTH;
	data_ray->size_hd = ANTIALIASING_FACT * ANTIALIASING_FACT * 3;
	data_ray->dx = 2 * data_ray->max_x / WIDTH;
	data_ray->hd_dx = data_ray->dx / ANTIALIASING_FACT;
}

void	average_hd_pixel(int hd_res[ANTIALIASING_FACT * ANTIALIASING_FACT][3])
{
	int		i;
	int		j;
	float	divisor;

	i = 0;
	while (++i < ANTIALIASING_FACT * ANTIALIASING_FACT)
	{
		j = -1;
		while (++j < 3)
			hd_res[0][j] += hd_res[i][j];
	}
	j = -1;
	divisor = (float)1 / (ANTIALIASING_FACT * ANTIALIASING_FACT);
	while (++j < 3)
		hd_res[0][j] *= divisor;
}

void	shoot_ray(t_data *data, t_shoot *shoot)
{
	float t;
	// isolate bounding box of interrest 
	// atomic_fetch_add(&num_primary_rays, 1);// perf logs
	t = visibility_intersection_tests(data->objects, shoot, data->n_obj);
	calculate_hit_pt(t, shoot);
	get_normal_intersect(shoot);
	shading(shoot, data);
}

void calculate_hit_pt(float t, t_shoot *shoot)
{
	if (t > EPSILON)
	{
		shoot->hit_pt[0] = shoot->src[0] + t * shoot->dir[0];
		shoot->hit_pt[1] = shoot->src[1] + t * shoot->dir[1];
		shoot->hit_pt[2] = shoot->src[2] + t * shoot->dir[2];
	}
	else
		shoot->obj = NULL;
}

void	iterate_hd_pxl(float *primary_rays, float dx, float top_left_x, float top_left_y)
{
	int i;
	int j;
	
	i = 0;
	while (i < ANTIALIASING_FACT)
	{
		// optimize calc i * WIDTH here ...
		j = 0;
		while (j < ANTIALIASING_FACT)
		{	
			primary_rays[i * ANTIALIASING_FACT * 3 + j * 3 + 0] = top_left_x + (j + 0.5) * dx;
			primary_rays[i * ANTIALIASING_FACT * 3 + j * 3 + 1] = top_left_y - (i + 0.5) * dx;
			primary_rays[i * ANTIALIASING_FACT * 3 + j * 3 + 2] = -1;
			normalize(&primary_rays[i * ANTIALIASING_FACT * 3 + j * 3]);
			j++;
		}
		i++;
	}
}

void get_normal_intersect(t_shoot *shoot)
{
	// we need to consider if we hit the back side of the surface !!! 
	
	float r_inv;
	
	if (!shoot->obj)
		return ;
	if (shoot->obj->type == SPHERE)
	{
		r_inv = 1 / ((t_sphere *)(shoot->obj->geo))->radius;
		shoot->normal[0] = r_inv * (shoot->hit_pt[0] - ((t_sphere *)(shoot->obj->geo))->center[0]);
		shoot->normal[1] = r_inv * (shoot->hit_pt[1] - ((t_sphere *)(shoot->obj->geo))->center[1]);
		shoot->normal[2] = r_inv * (shoot->hit_pt[2] - ((t_sphere *)(shoot->obj->geo))->center[2]);
	}
	else if (shoot->obj->type == PLANE)
	{
		shoot->normal[0] = ((t_plane *)(shoot->obj->geo))->normal[0];
		shoot->normal[1] = ((t_plane *)(shoot->obj->geo))->normal[1];
		shoot->normal[2] = ((t_plane *)(shoot->obj->geo))->normal[2];
	}
	else if (shoot->obj->type == CYLINDER)
	{
		int i = -1;
		float tmp[3];

		vec_substr(shoot->hit_pt, ((t_cylinder *)shoot->obj->geo)->center, tmp);
		float h = dot_13_13(tmp, ((t_cylinder *)shoot->obj->geo)->dir);
		while (++i < 3)
			tmp[i] = ((t_cylinder *)shoot->obj->geo)->center[i] + h * ((t_cylinder *)shoot->obj->geo)->dir[i];
		
		r_inv = 1 / ((t_cylinder *)(shoot->obj->geo))->radius;
		shoot->normal[0] = r_inv * (shoot->hit_pt[0] - tmp[0]);
		shoot->normal[1] = r_inv * (shoot->hit_pt[1] - tmp[1]);
		shoot->normal[2] = r_inv * (shoot->hit_pt[2] - tmp[2]);
	}
	// other SWITCHES ....
	if (dot_13_13(shoot->dir, shoot->normal) > 0)
	{
		shoot->normal[0] *= -1;
		shoot->normal[1] *= -1;
		shoot->normal[2] *= -1;
	}
}
