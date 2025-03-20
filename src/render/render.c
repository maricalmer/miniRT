/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:05:24 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/03/20 21:08:12 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	average_hd_pixel(int (*hd_res)[3], int hd_size);
void 	init_data_ray(t_ray_prim *data_ray, t_data *data);
void	calculate_pixel(t_calc_img_arg *arg, int i, t_shoot *shoot, int	(*hd_res)[3]);

void	render_first_image(t_data *data)
{
	clock_t	start;
	t_bvh	*bvh;

	start = clock(); //switch to get_time_of_day !!
	// move all that initialisation in a specific function.
	if (BVH_ON)
	{
		bvh = init_bvh(data); // rather return NULL  is no elements
		if (bvh->childs[0] != -2)
		{
			update_group(data, bvh);
			data->is_bvh = 1;
		}
		else
		{
			data->is_bvh = 0;
			free(bvh);
		}
	}
	// start of init_2
	data->cam.world_center[0] = (bvh->min_x[0] + bvh->max_x[0]) * 0.5;
	data->cam.world_center[1] = (bvh->min_y[0] + bvh->max_y[0]) * 0.5; 
	data->cam.world_center[2] = (bvh->min_z[0] + bvh->max_z[0]) * 0.5;
	cpy_vec(data->cam.origin, data->cam.origin_backup);
	data->mouse_pressed_l = 0;
	data->mouse_pressed_r = 0;
	// end of init_2
	first_rotation_matrice(data);
	print_bvh_build_t(start, clock());
	calculate_img(data);
}

void 	calculate_img(t_data *data)
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


void calc_p_ray(float x, float y, float res[3], float r_mat[3][3])
{
	res[0] = x;
	res[1] = y;
	res[2] = -1;
	normalize(res, NULL);
	dot_inplace_33_13(r_mat, res);
}

void 	calculate_img_packet(t_calc_img_arg *arg)
{
	t_shoot 		first_shoot;
	int				i;
	int				(*hd_res)[3]; 

	hd_res = malloc(sizeof(int [3]) * arg->data->antialiasing_fact
			* arg->data->antialiasing_fact);
	first_shoot.src = arg->data->cam.origin;
	first_shoot.depth = 0;
	first_shoot.inside = 0;
	i = -1;
	while (++i < WIDTH)
		calculate_pixel(arg, i, &first_shoot, hd_res);
	free(hd_res);
}
void	calculate_pixel(t_calc_img_arg *arg, int i, t_shoot *shoot, int	(*hd_res)[3])
{
	int				j;
	int				k;
	int				l;
	float			x;
	float			y;
	
	y = arg->y_tl - arg->dx_hd;
	j = -1;
	while (++j < arg->data->antialiasing_fact)
	{
		x = arg->x_tl + arg->dx * i + arg->dx_hd;
		k = -1;
		while (++k < arg->data->antialiasing_fact)
		{
			calc_p_ray(x, y, shoot->dir, arg->data->cam.r_mat);
			shoot_ray(arg->data, shoot);
			x += arg->dx_hd;
			l = -1;
			while (++l< 3)
				hd_res[j * arg->data->antialiasing_fact + k][l] = imin(shoot->res_rgb[l], 255);
		}
		y -= arg->dx_hd;
	}
	average_hd_pixel(hd_res, arg->data->antialiasing_fact * arg->data->antialiasing_fact);
	arg->data->mlx.addr[i + arg->line * WIDTH] = (hd_res[0][0] << 16 | hd_res[0][1] << 8 | hd_res[0][2]);
}


void init_data_ray(t_ray_prim *data_ray, t_data *data)
{
	data_ray->max_x = tan(data->cam.fov * M_PI / 360);
	data_ray->max_y = data_ray->max_x * HEIGHT / WIDTH;
	data_ray->size_hd = data->antialiasing_fact * data->antialiasing_fact * 3;
	data_ray->dx = 2 * data_ray->max_x / WIDTH;
	data_ray->hd_dx = data_ray->dx / data->antialiasing_fact;
}

void	average_hd_pixel(int (*hd_res)[3], int hd_size)
{
	int		i;
	int		j;
	float	divisor;

	i = 0;
	while (++i < hd_size)
	{
		j = -1;
		while (++j < 3)
			hd_res[0][j] += hd_res[i][j];
	}
	j = -1;
	divisor = (float)1 / (hd_size);
	while (++j < 3)
		hd_res[0][j] *= divisor;
}

