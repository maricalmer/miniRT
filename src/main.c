#include "minirt.h"

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
	render_first_image(&data, addr);
	mlx_put_image_to_window(mlx, win, img, 0, 0);
	mlx_loop(mlx);
}
