#include "minirt.h"

static void	init_data(t_data *data)
{
	data->n_obj = 0;
	data->n_light = 0;
	data->n_obj_files = 0;
	data->objects_idx = 0;
	data->obj_fd = -1;
	data->rt_fd = -1;
	data->exit = 0;
	data->anti_fa = ANTIALIASING_FACT;
	srand(123456);
}

// int main(void)
int main(int ac, char **av)
{
	t_data 		data;

	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
	print_intro();
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
    mlx_hook(data.mlx.win, 4, 1L << 2, &mouse_press, &data);
    mlx_hook(data.mlx.win, 5, 1L << 3, &mouse_release, &data);
    mlx_hook(data.mlx.win, 6, 1L << 6, &mouse_move, &data);
	mlx_hook(data.mlx.win, 17, 0, &handle_close, &data);
	mlx_loop(data.mlx.mlx);
	// free dyn alloc mem (incl. pthread_mutex_destroy)

	return (0);
}
