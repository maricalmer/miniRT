/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:50:02 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/17 22:15:32 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	handle_mouse_left(t_data *data, int x, int y);
static void	handle_mouse_right(t_data *data, int x, int y);

int	mouse_press(int button, int x, int y, void *arg)
{
	t_data	*data;

	data = arg;
	if (button == 1)
	{
		data->mouse_pressed_l = 1;
		data->mouse_x = x;
		data->mouse_y = y;
	}
	else if (button == 3)
	{
		data->mouse_pressed_r = 1;
		data->mouse_x = x;
		data->mouse_y = y;
	}
	else if (button == 4)
		translate_cam(data, data->cam.z, -data->cam.d_trans * 0.5f, 1);
	else if (button == 5)
		translate_cam(data, data->cam.z, data->cam.d_trans * 0.5f, 1);
	return (0);
}

int	mouse_release(int button, int x, int y, void *arg)
{
	t_data	*data;

	(void)x;
	(void)y;
	data = arg;
	if (button == 1)
	{
		data->mouse_pressed_l = 0;
		data->anti_fa = ANTIALIASING_FACT;
		calculate_img(data);
	}
	else if (button == 3)
	{
		data->mouse_pressed_r = 0;
		data->anti_fa = ANTIALIASING_FACT;
		calculate_img(data);
	}
	return (0);
}

int	mouse_move(int x, int y, void *arg)
{
	t_data	*data;

	data = arg;
	if (data->mouse_pressed_l)
		handle_mouse_left(data, x, y);
	if (data->mouse_pressed_r)
		handle_mouse_right(data, x, y);
	return (0);
}

static void	handle_mouse_left(t_data *data, int x, int y)
{
	int		dx;
	int		dy;
	float	v[3];
	float	amp;

	dx = x - data->mouse_x;
	dy = y - data->mouse_y;
	if ((dx * dx + dy * dy) < MOVE_THRESHOLD_2)
		return ;
	data->mouse_x = x;
	data->mouse_y = y;
	v[0] = -dx * data->cam.x[0] + dy * data->cam.y[0];
	v[1] = -dx * data->cam.x[1] + dy * data->cam.y[1];
	v[2] = -dx * data->cam.x[2] + dy * data->cam.y[2];
	normalize(v, NULL);
	amp = data->cam.d_trans * 0.5f;
	translate_cam(data, v, amp, 1);
}

static void	handle_mouse_right(t_data *data, int x, int y)
{
	int		dx;
	int		dy;
	float	v[3];

	dx = x - data->mouse_x;
	dy = y - data->mouse_y;
	if ((dx * dx + dy * dy) < MOVE_THRESHOLD_2)
		return ;
	data->mouse_x = x;
	data->mouse_y = y;
	v[0] = -dy * data->cam.x[0] - dx * data->cam.y[0];
	v[1] = -dy * data->cam.x[1] - dx * data->cam.y[1];
	v[2] = -dy * data->cam.x[2] - dx * data->cam.y[2];
	normalize(v, NULL);
	rotate_cam(data, CAM_D_THETA_MOUSE * M_PI / 180, v, 1);
}
