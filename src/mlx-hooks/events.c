/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:34:36 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/23 12:09:23 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	copy_r_mat(t_data *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < 3)
	{
		j = -1;
		while (++j < 3)
			data->cam.r_mat[i][j] = data->cam.t_mat[i][j];
	}
}

void	translate_cam(t_data *data, float v[3], float amp, int anti_fa)
{
	float	t[3];
	double	new_t_mat[4][4];

	data->anti_fa = anti_fa;
	cpy_vec(v, t);
	scale_vec(t, amp);
	ft_memset(new_t_mat, 0, sizeof(double [4][4]));
	new_t_mat[0][0] = 1;
	new_t_mat[1][1] = 1;
	new_t_mat[2][2] = 1;
	new_t_mat[3][3] = 1;
	new_t_mat[0][3] = t[0];
	new_t_mat[1][3] = t[1];
	new_t_mat[2][3] = t[2];
	dot_inplace_44_44(data->cam.t_mat, new_t_mat);
	copy_r_mat(data);
	ft_memcpy(data->cam.origin, data->cam.origin_backup, sizeof(float [3]));
	dot_inplace_34_13(data->cam.t_mat, data->cam.origin);
	calculate_img(data);
}

void	rotate_cam(t_data *data, float theta, float axis[3], int anti_fa)
{
	double new_t_mat[4][4]; // change that name
	
	data->anti_fa = anti_fa;
	rodrigues_matrice_handler(axis, theta, data->cam.world_center, new_t_mat);
	dot_inplace_44_44(data->cam.t_mat, new_t_mat);
	copy_r_mat(data);
	ft_memcpy(data->cam.origin, data->cam.origin_backup, sizeof(float [3]));
	dot_inplace_34_13(data->cam.t_mat, data->cam.origin);
	calculate_img(data);
}
