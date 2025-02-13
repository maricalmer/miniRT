#include "minirt.h"

void 	calculate_img(t_data *data, int *img);
void 	calculate_img_packet(void *arg_generic);
void	average_hd_pixel(int hd_res[ANTIALIASING_FACT * ANTIALIASING_FACT][3]);
void	calculate_ray_prim_dir(t_data *data);
void	calculate_ray_prim_dir_packet(void *arg_generic);
void	iterate_hd_pxl(float *primary_rays, float dx, float top_left_x, float top_left_y);
void 	visibility_intersection_tests(t_data *data, t_shoot *shoot);
void 	get_normal_intersect(t_shoot *shoot);

void	render_first_image(t_data *data, int *img) // img + mlx + win in data (??)
{
	data->primary_rays = malloc(sizeof(float) * HEIGHT * WIDTH * 3 * ANTIALIASING_FACT * ANTIALIASING_FACT);
	calculate_ray_prim_dir(data);   // ===> apply multithreading (x N_THREADS)
	t_aabb *root = init_bvh(data->objects);
	(void) root;
	calculate_img(data, img);
}


void 	calculate_img(t_data *data, int *img)
{
	int				i;
	t_calc_img_arg	*arg;
	int				packet_size;

	packet_size = WIDTH * HEIGHT / 256;
	arg = malloc(sizeof(t_calc_img_arg) * 256);
	pthread_mutex_lock(&data->joblist_mutex);
	data->joblist_top = 0;
	data->joblist_size = 256;
	i = -1;
	while (++i < 256)
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
}

// void calculate_ray_prim_dir(t_data *data)
// {
// 	float max_x;
// 	float max_y;
// 	float top_left_x;
// 	float top_left_y;
// 	float dx;
// 	float hd_dx;
// 	float cam_origin[3];
// 	int	i;
// 	int	j;
// 	int index;
// 	int size_hd;

// 	max_x = tan(data->cam.fov * M_PI / 360);
// 	ft_memset(cam_origin, 0, sizeof(float) * 3);
// 	max_y = max_x * HEIGHT / WIDTH;
// 	size_hd = ANTIALIASING_FACT * ANTIALIASING_FACT * 3;
	
// 	dx = 2 * max_x / WIDTH;
// 	hd_dx = dx / ANTIALIASING_FACT;
// 	i = 0;
// 	top_left_y = max_y;
// 	while (i < HEIGHT)
// 	{
// 		// optimize index with += instead of i * WIDTH here ...
// 		j = 0;
// 		top_left_x = - max_x;
// 		while (j < WIDTH)
// 		{	
// 			index = i * WIDTH * size_hd + j * size_hd;
// 			iterate_hd_pxl(&data->primary_rays[index], hd_dx, top_left_x, top_left_y);
// 			j++;
// 			top_left_x += dx;
			
// 			// cam rotation missing here...
// 		}
// 		i++;
// 		top_left_y -= dx;
// 	}
// }

void	calculate_ray_prim_dir(t_data *data)  // duplicate of calculate_img: abstract to handler function at the cost of switch?
{
	int				i;
	t_calc_img_arg	*arg;
	int				packet_size;

	packet_size = WIDTH * HEIGHT / N_THREAD;
	arg = malloc(sizeof(t_calc_img_arg) * N_THREAD);
	pthread_mutex_lock(&data->joblist_mutex);
	data->joblist_top = 0;
	data->joblist_size = N_THREAD;
	i = -1;
	while (++i < N_THREAD)
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
	float max_x;
	float max_y;
	float top_left_x;
	float top_left_y;
	float dx;
	float hd_dx;
	int	i;
	int	j;
	int index;
	int size_hd;
	int start_x;
	int end_y;
	t_calc_img_arg 	*arg;
	arg = arg_generic;

	end_y = (arg->end / WIDTH);
	start_x = (arg->start % WIDTH);
	max_x = tan(arg->data->cam.fov * M_PI / 360);
	max_y = max_x * HEIGHT / WIDTH;
	size_hd = ANTIALIASING_FACT * ANTIALIASING_FACT * 3;

	dx = 2 * max_x / WIDTH;
	hd_dx = dx / ANTIALIASING_FACT;
	i = arg->start / WIDTH;
	top_left_y = max_y - (i * dx);
	while (i < end_y)
	{
		j = start_x;
		top_left_x = -max_x + (j * dx);
		while (j < WIDTH)
		{
			index = i * WIDTH * size_hd + j * size_hd;
			iterate_hd_pxl(&arg->data->primary_rays[index], hd_dx, top_left_x, top_left_y);
			j++;
			top_left_x += dx;

			// cam rotation missing here...
		}
		i++;
		top_left_y -= dx;
	}
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
	// isolate bounding box of interrest 
	//atomic_fetch_add(&num_primary_rays, 1);// perf logs
	visibility_intersection_tests(data, shoot);
	get_normal_intersect(shoot);
	shading(shoot, data);
}


void visibility_intersection_tests(t_data *data, t_shoot *shoot)
{
	t_object *obj;
	float t_min;
	float t;

	obj = data->objects;
	t_min = FLT_MAX;

	t = 0;

	//num_object_tests++;
	//atomic_fetch_add(&num_object_tests, 1); // perf logs
	while (obj->type)
	{
		if (obj->type == SPHERE)
			t = intersection_test_sphere(obj->geo, shoot->dir, shoot->src);
		else if (obj->type == PLANE)
		 	t = intersection_test_plane(obj->geo, shoot->dir, shoot->src);
		if (t > EPSILON && t < t_min)
		{
			t_min = t;
			shoot->obj = obj;
		}
		obj++;
	}
	if (t_min != FLT_MAX)
	{
		shoot->hit_pt[0] = shoot->src[0] + t_min * shoot->dir[0];
		shoot->hit_pt[1] = shoot->src[1] + t_min * shoot->dir[1];
		shoot->hit_pt[2] = shoot->src[2] + t_min * shoot->dir[2];
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

// duplicate function for shadow test for optimization: if discr >= 0 return 1
float	intersection_test_sphere(t_sphere *sphere, float p_ray[3], float origin[3])
{
	float	c_minus_o[3];
	float	a;
	float	b;
	float	c;
	float	discriminant;
	float	res[2];

	//num_object_intersections++;
	//atomic_fetch_add(&num_object_intersections, 1); // perf logs
	
	c_minus_o[0] = sphere->center[0] - origin[0];
	c_minus_o[1] = sphere->center[1] - origin[1];
	c_minus_o[2] = sphere->center[2] - origin[2];

	a = dot_13_13(p_ray, p_ray);

	b = -2 * dot_13_13(p_ray, c_minus_o);
	c = dot_13_13(c_minus_o, c_minus_o) - sphere->radius * sphere->radius;

	discriminant = b * b - 4 * a * c;

	if (discriminant < 0)
		return (0);
	discriminant = sqrtf(discriminant);
	res[0] = (-b - discriminant) / (2 * a);
	res[1] = (-b + discriminant) / (2 * a);
	if (res[0] > 0)
		return(res[0]);
	if (res[1] > 0)
		return(res[1]);
	return (0);
}

float	intersection_test_plane(t_plane *plane, float p_ray[3], float origin[3])
{
	float	a;
	float	pts_d[3];
	float	b;

	//num_object_intersections++;
	//atomic_fetch_add(&num_object_intersections, 1); // perf logs

	a = dot_13_13(plane->normal, p_ray);
	if (a < EPSILON && a > -EPSILON)
		return (0);
	pts_d[0] = plane->point[0] - origin[0];
	pts_d[1] = plane->point[1] - origin[1];
	pts_d[2] = plane->point[2] - origin[2];
	b = dot_13_13(plane->normal, pts_d);
	if ((a < 0 && b > 0) || (a > 0 && b < 0)) // performs better than if (b > 0) after (b / a)
		return (0);
	return (b / a);
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
	// other SWITCHES ....
}
