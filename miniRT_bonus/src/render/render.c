/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:05:24 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/03/26 13:38:08 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	init_data_ray(t_ray_prim *data_ray, t_data *data);

void	render_first_image(t_data *data)
{
	t_bvh	*bvh;

	bvh = init_bvh(data);
	data->cam.world_center[0] = (bvh->min_x[0] + bvh->max_x[0]) * 0.5;
	data->cam.world_center[1] = (bvh->min_y[0] + bvh->max_y[0]) * 0.5; 
	data->cam.world_center[2] = (bvh->min_z[0] + bvh->max_z[0]) * 0.5;
	data->cam.d_trans = (-bvh->min_x[0] + bvh->max_x[0] - bvh->min_y[0]
			+ bvh->max_y[0] - bvh->min_z[0] + bvh->max_z[0]) / 3 / TRANS_SENS;
	if (data->cam.d_trans < EPSILON)
		data->cam.d_trans = 10;
	cpy_vec(data->cam.origin, data->cam.origin_backup);
	first_rotation_matrice(data);
	calculate_img(data);
}

void	calculate_img(t_data *data)
{
	int				i;
	t_ray_prim		data_ray;
	struct timeval	t_start;
	struct timeval	t_end;

	gettimeofday(&t_start, NULL);
	init_data_ray(&data_ray, data);
	pthread_mutex_lock(&data->joblist_mutex);
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
	}
	pthread_mutex_unlock(&data->joblist_mutex);
	wait_for_workers(data);
	mlx_put_image_to_window(data->mlx.mlx, data->mlx.win, data->mlx.img, 0, 0);
	mlx_do_sync(data->mlx.mlx);
	gettimeofday(&t_end, NULL);
	print_img_render_t(t_start, t_end);
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
