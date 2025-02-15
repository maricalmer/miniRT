#include "minirt.h"

atomic_int 	num_primary_rays = 0;
atomic_int 	num_object_tests = 0;
atomic_int 	num_object_intersections = 0;
int 		total_objects = 0;
clock_t 	start  = 0;
double time_primary_rays = 0;
double time_visibility_test = 0;
double time_normal_intersect = 0;
double time_shading = 0;
double render_start = 0;
double time_total_render = 0;

int main(void)
{
	t_data 		data;
	t_mlxlib	mlx_data;
    

	parsing(&data); // argv
	int i = -1;
	while (data.objects[++i].type)
		total_objects++;
	launch_pool(&data);
	if (init_mlx(&mlx_data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	render_first_image(&data, mlx_data.addr);
	mlx_put_image_to_window(mlx_data.mlx, mlx_data.win, mlx_data.img, 0, 0);
	mlx_key_hook(mlx_data.win, &handle_input, &mlx_data);
	mlx_hook(mlx_data.win, CROSS_CLICK_EVENT, NO_EVENT_MASK, &handle_close, &mlx_data);
	mlx_loop(mlx_data.mlx);
	//free dyn alloc mem (incl. pthread_mutex_destroy)
	return (0);
}
