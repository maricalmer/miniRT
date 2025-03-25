/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:05:24 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/03/25 11:52:04 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	init_data_ray(t_ray_prim *data_ray, t_data *data);

void	render_first_image(t_data *data)
{
	data->cam.world_center[0] = 0;
	data->cam.world_center[1] = 0; 
	data->cam.world_center[2] = 0;
	data->cam.d_trans = 10;
	cpy_vec(data->cam.origin, data->cam.origin_backup);
	first_rotation_matrice(data);
	calculate_img(data);
}

void	calculate_img(t_data *data)
{
	int				i;
	t_ray_prim		data_ray;

	init_data_ray(&data_ray, data);
	data->joblist_top = 0;
	i = -1;
	while (++i < HEIGHT)
	{
		data->joblist[i].x_tl = -data_ray.max_x;
		data->joblist[i].y_tl = data_ray.max_y - data_ray.dx * i;
		data->joblist[i].dx = data_ray.dx;
		data->joblist[i].dx_hd = data_ray.hd_dx;
		data->joblist[i].data = data;
		data->joblist[i].line = i;
		calculate_img_packet(&data->joblist[i]);
	}
	mlx_put_image_to_window(data->mlx.mlx, data->mlx.win, data->mlx.img, 0, 0);
	mlx_do_sync(data->mlx.mlx);
}

static void	init_data_ray(t_ray_prim *data_ray, t_data *data)
{
	data_ray->max_x = tan(data->cam.fov * M_PI / 360);
	data_ray->max_y = data_ray->max_x * HEIGHT / WIDTH;
	data_ray->dx = 2 * data_ray->max_x / WIDTH;
	data_ray->hd_dx = data_ray->dx / data->anti_fa;
}

void	calculate_img_packet(t_calc_img_arg *arg)
{
	t_shoot			first_shoot;
	int				i;
	int				(*hd_res)[3]; 

	hd_res = malloc(sizeof(int [3]) * arg->data->anti_fa
			* arg->data->anti_fa);
	if (!hd_res)
		exit(EXIT_FAILURE);
	first_shoot.src = arg->data->cam.origin;
	first_shoot.depth = 0;
	first_shoot.inside = 0;
	first_shoot.intens = 1;
	i = -1;
	while (++i < WIDTH)
		calculate_pixel(arg, i, &first_shoot, hd_res);
	free(hd_res);
}
