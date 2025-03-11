#include "minirt.h"

void copy_r_mat(t_data *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < 3)
	{
		j = -1;
		while (++j < 3)
			data->cam.r_mat[i][j] = data->cam.t_mat[i][j];
	}
}

void translate_cam(t_data *data, float v[3], float amp)
{
	float t[3];
	double R[4][4]; // change that name

	cpy_vec(v, t);
	scale_vec(t, amp);
	vec_add_inplace(data->cam.origin, t);
	vec_add_inplace(data->cam.world_center, t);
	ft_memset(R, 0, sizeof(double[4][4]));
	R[0][0] = R[1][1] = R[2][2] = R[3][3] = 1;
	R[0][3] = t[0];
	R[1][3] = t[1];
	R[2][3] = t[2];
	dot_inplace_44_44(data->cam.t_mat, R);
	copy_r_mat(data);
	calculate_img(data);
}

void rotate_cam(t_data *data, float theta, char axis)
{
	double R[4][4]; // change that name
	
	if (axis == 'x')
		rodrigues_matrice_handler((float[3]){1, 0, 0}, theta, data->cam.world_center, R);
	if (axis == 'y')
		rodrigues_matrice_handler((float[3]){0, 1, 0}, theta, data->cam.world_center, R);
	dot_inplace_44_44(data->cam.t_mat, R); // update the global transformation matrice
	copy_r_mat(data);
	ft_memcpy(data->cam.x, (float[3]){1,0,0}, sizeof(float[3]));
	ft_memcpy(data->cam.y, (float[3]){0,1,0}, sizeof(float[3]));
	cpy_vec(data->cam.direction_backup, data->cam.direction);
	dot_inplace_33_13(data->cam.r_mat, data->cam.x);
	dot_inplace_33_13(data->cam.r_mat, data->cam.y);
	dot_inplace_33_13(data->cam.r_mat, data->cam.direction);
	ft_memcpy(data->cam.origin, data->cam.origin_backup, sizeof(float[3]));
	dot_inplace_34_13(data->cam.t_mat, data->cam.origin);
	// move_cam_origin(data->cam.origin_backup, data->cam.t_mat, data->cam.world_center, data->cam.origin);
	calculate_img(data);
}
/*
void select_obj(t_data *data, int x_click, int y_click)
{
	t_shoot	shoot;

	data->mode = OBJECT_MODE;
	OPT 1 : if (data->selected_obj != NULL)
		rebuild bvh
	// find_p_ray(x_click, y_click), or recalalculate ??!!!!;
	// initialize shoot;
	visibility_intersection_tests(data->objects, &shoot, data->n_obj);
	data->selected_obj = shoot.obj;
	OPT 1 : exclude obj from bvh (type = SELECTED); add obj to data->objects[max]; 
}

void rotate_obj(t_data data, float x, float y)
{
	// switch to rotate corresponding primitive or all triangles of a trimesh ...;
	(OPT 2 : rebuild bvh)
	calculate_img(data, img);
}

void translate_obj(t_data data, float x, float y)
{
	// switch to translate corresponding primitive or all triangles of a trimesh ...;
	(OPT 2 : rebuild bvh)
	calculate_img(data, img);
}

void scale_obj(t_data data, float factor)
{
	// switch to scale corresponding primitive or all triangles of a trimesh ...;
	(OPT 2 : rebuild bvh)
	calculate_img(data, img);
}


void exit_key(t_data *data)
{
	if (data->mode = OBJECT_MODE)
		data->mode = CAMERA_MODE;
	else
	{
		existing exit...
		+ clean_mem(), thread and mutex clean...
	}
}

*/
