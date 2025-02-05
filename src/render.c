#include "minirt.h"

void calculate_ray_prim_dir(t_data *data);
void build_screen_frame(t_data *data);
void normalize(float vector[3]);
void intersection_tests(t_data *data, t_intersect *first, int i);
float	intersection_test_sphere(t_sphere *sphere, float p_ray[3], float origin[3]);
float	intersection_test_plane(t_plane *plane, float p_ray[3], float origin[3]);
void get_normal_intersect(t_intersect *first);

void	render_first_image(t_data *data, int *img)
{
	t_intersect 	first;
	
	calculate_ray_prim_dir(data); // => toutes les directions des primary rays are saved.
	int i = 0;
	int space = HEIGHT * WIDTH;
	while (i < space)
	{
		intersection_tests(data, &first, i); // => the first inter point is returned
		get_normal_intersect(&first);
		//norm = calculate_normal(data, obj, hit_pt);
		img[i] = phong(&first, data); // => color = object color in the first prototype.)
		
		
		i++;
	}
}

void intersection_tests(t_data *data, t_intersect *first, int i)
{
	t_object *obj;
	float t_min;
	float t;

	obj = data->objects;
	t_min = FLT_MAX;

	while (obj->type)
	{
		if (obj->type == SPHERE)
			t = intersection_test_sphere(obj->data, data->primary_rays[i / WIDTH][i % WIDTH], data->cam.origin); // world_space , return 0 if no intersection
		else if (obj->type == PLANE)
		 	t = intersection_test_plane(obj->data, data->primary_rays[i / WIDTH][i % WIDTH], data->cam.origin); // world_space , return 0 if no intersection
		if (t > EPSILON && t < t_min)
		{
			t_min = t;
			first->obj = obj;
		}
		obj++;
	}
	if (t_min != FLT_MAX)
	{
		first->hit_pt[0] = data->cam.origin[0] + t_min * data->primary_rays[i / WIDTH][i % WIDTH][0]; // conversionn needed form 2D to 1D (see build_screen_frame())
		first->hit_pt[1] = data->cam.origin[1] + t_min * data->primary_rays[i / WIDTH][i % WIDTH][1];
		first->hit_pt[2] = data->cam.origin[2] + t_min * data->primary_rays[i / WIDTH][i % WIDTH][2];
	}
	else
		first->obj = NULL;
}

/*
calculate the primary ray direction for every pixel in our frame;
save the result in data;
*/
void calculate_ray_prim_dir(t_data *data)
{	
	build_screen_frame(data);

	/*
	
	p_center = calculate_pixel_center(frame_coord);
	data->prim_ray_dir[i] = norm_vect(cam_orig, p_center);*/
}


void build_screen_frame(t_data *data)
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
		j = 0;
		while (j < WIDTH)
		{
			data->primary_rays[i][j][0] = -max_x + (j + 0.5) * dx;
			data->primary_rays[i][j][1] = max_y - (i + 0.5) * dx;
			data->primary_rays[i][j][2] = -1;
			normalize(data->primary_rays[i][j]);
//			data->primary_rays[i][j] = points_to_n_vector(cam_origin, data->primary_rays[i][j]);
			j++;
		}
		i++;
	}

}

void normalize(float vector[3])
{
	float temp;
	float magnitude;

	magnitude = sqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
	temp = 1 / magnitude;
	vector[0] *= temp;
	vector[1] *= temp;
	vector[2] *= temp;
}
/*
float *points_to_n_vector(float p1[3], float p2[3])
{
	
}
*/
float	dot_13_13(float a[3], float b[3])
{
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}

float	intersection_test_sphere(t_sphere *sphere, float p_ray[3], float origin[3])
{
	float	c_minus_o[3];
	float	a;
	float	b;
	float	c;
	float	b24ac;
	float	res[2];

	c_minus_o[0] = sphere->center[0] - origin[0];
	c_minus_o[1] = sphere->center[1] - origin[1];
	c_minus_o[2] = sphere->center[2] - origin[2];

	a = dot_13_13(p_ray, p_ray);

	b = -2 * dot_13_13(p_ray, c_minus_o);
	c = dot_13_13(c_minus_o, c_minus_o) - sphere->radius * sphere->radius;

	b24ac = b * b - 4 * a * c;

	if (b24ac < 0)
		return (0);
	b24ac = sqrtf(b24ac);
	res[0] = (-b - b24ac) / (2 * a);
	res[1] = (-b + b24ac) / (2 * a);
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

void get_normal_intersect(t_intersect *first)
{
	float r_inv;
	
	if (!first->obj)
		return ;
	if (first->obj->type == SPHERE)
	{
		r_inv = 1 / ((t_sphere *)(first->obj->data))->radius;
		first->normal[0] = r_inv * (first->hit_pt[0] - ((t_sphere *)(first->obj->data))->center[0]);
		first->normal[1] = r_inv * (first->hit_pt[1] - ((t_sphere *)(first->obj->data))->center[1]);
		first->normal[2] = r_inv * (first->hit_pt[2] - ((t_sphere *)(first->obj->data))->center[2]);
	}
	else if (first->obj->type == PLANE)
	{
		first->normal[0] = ((t_plane *)(first->obj->data))->normal[0];
		first->normal[1] = ((t_plane *)(first->obj->data))->normal[1];
		first->normal[2] = ((t_plane *)(first->obj->data))->normal[2];
	}
	// other SWITCHES ....
}





















