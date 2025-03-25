/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:34:56 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/25 11:54:59 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	handle_input_2(int keysym, t_data *data);
static void	free_mlx(t_mlxlib *vars);

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
	data->addr = (int *)mlx_get_data_addr(data->img, &data->bpp, 
			&data->line_len, &data->endian);
	if (!data->addr)
	{
		mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	handle_close(t_data *data)
{
	free_mlx(&data->mlx);
	free_data(data);
	print_outro();
	exit(EXIT_SUCCESS);
	return (0);
}

int	handle_input(int keysym, t_data *data)
{
	if (keysym == 'q')
		handle_close(data);
	else if (keysym == XK_Escape)
		handle_close(data);
	else if (keysym == 'a')
		translate_cam(data, data->cam.x, data->cam.d_trans, 1);
	else if (keysym == 'd')
		translate_cam(data, data->cam.x, -data->cam.d_trans, 1);
	else if (keysym == 'w')
		translate_cam(data, data->cam.y, -data->cam.d_trans, 1);
	else if (keysym == 's')
		translate_cam(data, data->cam.y, data->cam.d_trans, 1);
	else if (keysym == 'z')
		translate_cam(data, data->cam.z, -data->cam.d_trans, 1);
	else if (keysym == 'x')
		translate_cam(data, data->cam.z, data->cam.d_trans, 1);
	else
		handle_input_2(keysym, data);
	return (0);
}

static void	handle_input_2(int keysym, t_data *data)
{
	if (keysym == 'j')
		rotate_cam(data, CAM_D_THETA * M_PI / 180, data->cam.y, 1);
	else if (keysym == 'l')
		rotate_cam(data, -CAM_D_THETA * M_PI / 180, data->cam.y, 1);
	else if (keysym == 'i')
		rotate_cam(data, CAM_D_THETA * M_PI / 180, data->cam.x, 1);
	else if (keysym == 'k')
		rotate_cam(data, -CAM_D_THETA * M_PI / 180, data->cam.x, 1);
	else if (keysym == 'u')
		rotate_cam(data, CAM_D_THETA * M_PI / 180, data->cam.z, 1);
	else if (keysym == 'o')
		rotate_cam(data, -CAM_D_THETA * M_PI / 180, data->cam.z, 1);
	else if (keysym == 'c')
		data->cam.mode = !data->cam.mode;
	else if (keysym == 'm')
	{
		data->anti_fa = ANTIALIASING_FACT;
		calculate_img(data);
	}
}

static void	free_mlx(t_mlxlib *vars)
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
