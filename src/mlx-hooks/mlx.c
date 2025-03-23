/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:34:56 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/23 13:56:58 by hruiz-fr         ###   ########.fr       */
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
	if (1)
		free_bvh_2(data->objects[0].geo.bvh);
	free_data(data);
	join_threads(data);
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
		translate_cam(data, (float [3]){1, 0, 0}, -CAM_D_TRANS, 1);
	else if (keysym == 'd')
		translate_cam(data, (float [3]){1, 0, 0}, CAM_D_TRANS, 1);
	else if (keysym == 'w')
		translate_cam(data, (float [3]){0, 1, 0}, -CAM_D_TRANS, 1);
	else if (keysym == 's')
		translate_cam(data, (float [3]){0, 1, 0}, CAM_D_TRANS, 1);
	else if (keysym == 'z')
		translate_cam(data, (float [3]){0, 0, 1}, -CAM_D_TRANS, 1);
	else if (keysym == 'x')
		translate_cam(data, (float [3]){0, 0, 1}, CAM_D_TRANS, 1);
	else
		handle_input_2(keysym, data);
	return (0);
}

static void	handle_input_2(int keysym, t_data *data)
{
	if (keysym == 'j')
		rotate_cam(data, 15 * M_PI / 180, (float [3]){0, 1, 0}, 1);
	else if (keysym == 'l')
		rotate_cam(data, -15 * M_PI / 180, (float [3]){0, 1, 0}, 1);
	else if (keysym == 'i')
		rotate_cam(data, 15 * M_PI / 180, (float [3]){1, 0, 0}, 1);
	else if (keysym == 'k')
		rotate_cam(data, -15 * M_PI / 180, (float [3]){1, 0, 0}, 1);
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
