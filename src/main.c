#include "minirt.h"

atomic_int num_primary_rays = 0;
atomic_int num_object_tests = 0;
atomic_int num_object_intersections = 0;
int total_objects = 0;

int main(void)
{
	void *mlx;
	void *win;
	void *img;
	int *addr;
	int bpp;
	int line_len;
	int endian;
	t_data data;
	//clock_t start;
    
    //start_timer(&start);

	ft_printf("hello\n");
	mlx = mlx_init();
	if (!mlx)
		return (EXIT_FAILURE);
	win = mlx_new_window(mlx, WIDTH, HEIGHT, "miniRT-beta");
	if (!win)
	{
		mlx_destroy_display(mlx);
		return (EXIT_FAILURE);
	}
	img = mlx_new_image(mlx, WIDTH, HEIGHT);

	addr = (int *)mlx_get_data_addr(img, &bpp, &line_len, &endian);
	if (!addr)
	{
		mlx_destroy_window(mlx, win);
		mlx_destroy_display(mlx);
		return (EXIT_FAILURE);
	}
	parsing(&data); // argv
	launch_pool(&data);
	render_first_image(&data, addr);
	mlx_put_image_to_window(mlx, win, img, 0, 0);
	mlx_loop(mlx);
	//double render_time = stop_timer(start);
	//print_render_stats(render_time);
	// free dyn alloc mem (incl. pthread_mutex_destroy)
	return (0);
}
