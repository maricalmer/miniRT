/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_pixel.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:42:06 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 16:57:05 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Per-pixel ray calculation for image rendering. Handles anti-aliasing by    */
/* shooting multiple rays per pixel, averaging their color values for         */
/* smoother results. Computes ray directions, fires into scene, stores final  */
/* pixel color.                                                               */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	calculate_primary_ray(float x, float y, float res[3], 
				float r_mat[3][3]);
static void	save_shoot_res_in_hd_res(t_shoot *shoot, int hd_res[3]);
static void	average_hd_pixel(int (*hd_res)[3], int hd_size);

void	render_pixel(t_calc_img_arg *arg, int p,
	t_shoot *shoot, int (*hd_res)[3])
{
	int				i;
	int				j;
	float			x;
	float			y;

	y = arg->y_tl - arg->dx_hd;
	i = -1;
	while (++i < arg->data->anti_fa)
	{
		x = arg->x_tl + arg->dx * p + arg->dx_hd;
		j = -1;
		while (++j < arg->data->anti_fa)
		{
			calculate_primary_ray(x, y, shoot->dir, arg->data->cam.r_mat);
			shoot_ray(arg->data, shoot);
			x += arg->dx_hd;
			save_shoot_res_in_hd_res(shoot, hd_res[i * arg->data->anti_fa + j]);
		}
		y -= arg->dx_hd;
	}
	average_hd_pixel(hd_res, arg->data->anti_fa * arg->data->anti_fa);
	arg->data->mlx.addr[p + arg->line * WIDTH]
		= (hd_res[0][0] << 16 | hd_res[0][1] << 8 | hd_res[0][2]);
}

static void	calculate_primary_ray(float x, float y, float res[3],
									float r_mat[3][3])
{
	res[0] = x;
	res[1] = y;
	res[2] = -1;
	normalize(res, NULL);
	matrix33_vec3_multiply_inplace(r_mat, res);
}

static void	save_shoot_res_in_hd_res(t_shoot *shoot, int hd_res[3])
{
	int	i;

	i = -1;
	while (++i < 3)
		hd_res[i] = imin(shoot->res_rgb[i], 255);
}

static void	average_hd_pixel(int (*hd_res)[3], int hd_size)
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
