#include "minirt.h"

int 		total_objects = 0;
clock_t 	start  = 0;
double time_primary_rays = 0;
double time_visibility_test = 0;
double time_normal_intersect = 0;
double time_shading = 0;
double render_start = 0;
double time_total_render = 0;

static void	init_data(t_data *data)
{
	data->n_obj = 0;
	data->n_light = 0;
	data->n_obj_files = 0;
	data->objects_idx = 0;
	data->obj_fd = -1;
	data->rt_fd = -1;
	data->exit = 0;
}

// int main(void)
int main(int ac, char **av)
{
	t_data 		data;

	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
	printf("triangle : %li \n", sizeof(t_triangle));
	printf("mat : %li \n", sizeof(t_material));
	printf("object _size : %li \n", sizeof(t_object));
	printf("bvh size : %li \n", sizeof(t_bvh));
	printf("t_shoot_size : %li \n", sizeof(t_shoot));
	printf("bvh max size : %i \n", BVH_SIZE_MAX);
	init_data(&data);
	if (check_input(ac, av, &data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (handle_parsing(av, &data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	// parsing(&data); // argv
	// int i = -1;
	// while (data.objects[++i].type)
	// 	total_objects++;
	launch_pool(&data);
	if (init_mlx(&data.mlx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	render_first_image(&data);
	mlx_key_hook(data.mlx.win, &handle_input, &data);
    mlx_hook(data.mlx.win, 4, 0, &mouse_press, &data);
    mlx_hook(data.mlx.win, 5, 0, &mouse_release, &data);
    mlx_hook(data.mlx.win, 6, (1L << 6), &mouse_move, &data);
	mlx_loop(data.mlx.mlx);
	// free dyn alloc mem (incl. pthread_mutex_destroy)

	return (0);
}
