#include "minirt.h"

// atomic_int 	num_primary_rays = 0;
// atomic_int 	num_object_tests = 0;
// atomic_int 	num_object_intersections = 0;
int 		total_objects = 0;
clock_t 	start  = 0;
double time_primary_rays = 0;
double time_visibility_test = 0;
double time_normal_intersect = 0;
double time_shading = 0;
double render_start = 0;
double time_total_render = 0;

// int main(void)
int main(int ac, char **av)
{
	t_data 		data;
	t_scn 		scn;

	if (check_input(ac, av, &scn) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	handle_parsing(&scn, av[1]);
	parsing(&data); // argv
	// int i = -1;
	// while (data.objects[++i].type)
	// 	total_objects++;
	launch_pool(&data);
	if (init_mlx(&data.mlx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	render_first_image(&data, data.mlx.addr);
	mlx_key_hook(data.mlx.win, &handle_input, &data.mlx);
	// mlx_loop(data.mlx.mlx);
	// free dyn alloc mem (incl. pthread_mutex_destroy)
	return (0);
}
