#include "minirt.h"
// david: is memcpy usage fixing bugs??

void 	calculate_img(t_data *data, int *img);
void	calculate_ray_prim_dir(t_data *data);
void 	normalize(float vector[3]);
void 	visibility_intersection_tests(t_data *data, t_shoot *shoot);
void 	get_normal_intersect(t_shoot *shoot);

void	render_first_image(t_data *data, int *img) // img + mlx + win in data (??)
{
	data->primary_rays = malloc(sizeof(float) * HEIGHT * WIDTH * 3);
	calculate_ray_prim_dir(data);
	calculate_img(data, img);
}


void 	calculate_img(t_data *data, int *img)
{
	t_shoot 	first_shoot;
	int			space;
	int			i;
	int			j;

	ft_memcpy(first_shoot.src, data->cam.origin, sizeof(first_shoot.src));
	first_shoot.depth = 0;
	i = 0;
	space = WIDTH * HEIGHT;
	while (i < space)
	{
		ft_memcpy(first_shoot.dir, &data->primary_rays[i * 3], sizeof(first_shoot.dir));
		
		
		shoot_ray(data, &first_shoot);
		j = -1;
		while (++j < 3)
			first_shoot.res_rgb[j] = imin(first_shoot.res_rgb[j], 255);
		img[i] = (first_shoot.res_rgb[0] << 16 | first_shoot.res_rgb[1] << 8 | first_shoot.res_rgb[2]);
		i++;
	}
}

void	shoot_ray(t_data *data, t_shoot *shoot)
{
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


void calculate_ray_prim_dir(t_data *data)
{
	float max_x;
	float max_y;
	float dx;
	float cam_origin[3];
	int	i;
	int	j;

	max_x = tan(data->cam.fov * M_PI / 360);

	ft_memset(cam_origin, 0, sizeof(float) * 3);
	max_y = max_x * HEIGHT / WIDTH;
	dx = 2 * max_x / WIDTH;
	i = 0;
	while (i < HEIGHT)
	{
		// optimize calc i * WIDTH here ...
		j = 0;
		while (j < WIDTH)
		{
			data->primary_rays[i * WIDTH * 3 + j * 3 + 0] = -max_x + (j + 0.5) * dx;
			data->primary_rays[i * WIDTH * 3 + j * 3 + 1] = max_y - (i + 0.5) * dx;
			data->primary_rays[i * WIDTH * 3 + j * 3 + 2] = -1;
			normalize(&data->primary_rays[i * WIDTH * 3 + j * 3]);
			// cam rotation missing here...
			j++;
		}
		i++;
	}
}



// float	intersection_test_sphere(t_sphere *sphere, t_shoot *shoot)
// {
// 	float	c_minus_o[3];
// 	float	a;
// 	float	b;
// 	float	c;
// 	float	discriminant;
// 	float	res[2];

// 	c_minus_o[0] = sphere->center[0] - shoot->src[0];
// 	c_minus_o[1] = sphere->center[1] - shoot->src[1];
// 	c_minus_o[2] = sphere->center[2] - shoot->src[2];

// 	a = dot_13_13(shoot->dir, shoot->dir);

// 	b = -2 * dot_13_13(shoot->dir, c_minus_o);
// 	c = dot_13_13(c_minus_o, c_minus_o) - sphere->radius * sphere->radius;

// 	discriminant = b * b - 4 * a * c;

// 	if (discriminant < 0)
// 		return (0);
// 	discriminant = sqrtf(discriminant);
// 	res[0] = (-b - discriminant) / (2 * a);
// 	res[1] = (-b + discriminant) / (2 * a);
// 	if (res[0] > 0)
// 		return(res[0]);
// 	if (res[1] > 0)
// 		return(res[1]);
// 	return (0);
// }

// duplicate function for shadow test for optimization: if discr >= 0 return 1
float	intersection_test_sphere(t_sphere *sphere, float p_ray[3], float origin[3])
{
	float	c_minus_o[3];
	float	a;
	float	b;
	float	c;
	float	discriminant;
	float	res[2];

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


// float	intersection_test_plane(t_plane *plane, t_shoot *shoot)
// {
// 	float	a;
// 	float	pts_d[3];
// 	float	b;

// 	a = dot_13_13(plane->normal, shoot->dir);
// 	if (a < EPSILON && a > -EPSILON)
// 		return (0);
// 	pts_d[0] = plane->point[0] - shoot->src[0];
// 	pts_d[1] = plane->point[1] - shoot->src[1];
// 	pts_d[2] = plane->point[2] - shoot->src[2];
// 	b = dot_13_13(plane->normal, pts_d);
// 	if ((a < 0 && b > 0) || (a > 0 && b < 0)) // performs better than if (b > 0) after (b / a)
// 		return (0);
// 	return (b / a);
// }

float	intersection_test_plane(t_plane *plane, float p_ray[3], float origin[3])
{
	float	a;
	float	pts_d[3];
	float	b;

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
