#include "minirt.h"

void calculate_ray_prim_dir(t_data *data);
void build_screen_frame(t_data *data);
void normalize(float vector[3]);

void	render_first_image(t_data *data)
{
	t_intersect 	first;
	
	calculate_ray_prim_dir(data); // => toutes les directions des primary rays are saved.
	int i = 0;
	int space = HEIGHT * WIDTH;
	while (i < space)
	{
		first = intersection_tests(data, i) // => the first inter point is returned
		
		//norm = calculate_normal(data, obj, hit_pt);
		img[i] = phong(data, ); // => color = object color in the first prototype.)
		
		
		i++;
	}
}

t_intersect intersection_tests(t_data *data, int i)
{
	t_intersect res;
	t_object *obj;
	float t_min;
	float t;

	obj = data->objects;
	t_min = FLT_MAX;

	while (obj->type)
	{
		if (obj->type == SPHERE)
			t = intersection_test_sphere(); // world_space , return 0 if no intersection
		else if (obj->type == PLANE)
			t = intersection_test_plane(); // world_space , return 0 if no intersection
		if (t < EPSILON && t < t_min)
		{
			t_min = t;
			res.obj = obj;
		}
		obj++;
	}
	if (t_min < FLT_MAX)
		res.hit_pt = O + t_min * data->primary_rays[i]; // conversionn needed form 2D to 1D
	else
		 res.obj = NULL;
	return (res)

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
	
	max_x = tan(data->cam.fov* M_PI / 360);

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






























