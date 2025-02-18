#include "minirt.h"

void	free_map(t_mlxlib *vars)
{
	if (vars->mlx)
	{
		if (vars->win)
			mlx_destroy_window(vars->mlx, vars->win);
		if (vars->img)
			mlx_destroy_image(vars->mlx, vars->img);
		mlx_destroy_display(vars->mlx);
		free(vars->mlx);
	}
}

int	handle_input(int keysym, t_mlxlib *vars)
{
	if (keysym == 'q')
		handle_close(vars);
	if (keysym == XK_Escape)
		handle_close(vars);
	return (0);
}

int	handle_close(t_mlxlib *vars)
{
	free_map(vars);
	//print_render_stats(time_total_render);
	exit(EXIT_SUCCESS);
	return (0);
}

int	init_mlx(t_mlxlib	*data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		return (EXIT_FAILURE);
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "miniRT-beta");
	if (!data->win)
	{
		mlx_destroy_display(data->mlx);
		return (EXIT_FAILURE);
	}
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->addr = (int *)mlx_get_data_addr(data->img, &data->bpp, &data->line_len, &data->endian);
	if (!data->addr)
	{
		mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}