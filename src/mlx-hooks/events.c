#include "minirt.h"


void translate_cam(t_data *data, float v[3])
{
	data->cam.origin += v;
	calculate_img(data, img);
}

void rotate_cam(t_data *data, float x, float y)
{
	rotate_p_rays(data, x, y);
	calculate_img(data, img);
}

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




/*
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
	mlx_put_image_to_window(data->mlx.mlx, data->mlx.win, data->mlx.img, 0, 0);
}
*/