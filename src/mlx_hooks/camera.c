/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:34:36 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 16:05:50 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Handles camera transformations in the scene, including rotation and        */
/* translation. Updates the camera's transformation and rotation matrices     */
/* based on movement vectors or rotation angles, and recalculates the image.  */
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
	matrix33_matrix33_multiply_inplace(data->cam.r_mat, data->cam.r_mat_0);
}

void	copy_r_mat_0(t_data *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < 3)
	{
		j = -1;
		while (++j < 3)
		{
			data->cam.r_mat_0[i][j] = data->cam.t_mat[i][j];
			data->cam.r_mat[i][j] = data->cam.t_mat[i][j];
		}
	}
}

void	translate_cam(t_data *data, float v[3], float amp, int anti_fa)
{
	float	t[3];
	double	new_t_mat[4][4];

	data->anti_fa = anti_fa;
	copy_vec(v, t);
	scale_vec(t, amp);
	ft_memset(new_t_mat, 0, sizeof(double [4][4]));
	new_t_mat[0][0] = 1;
	new_t_mat[1][1] = 1;
	new_t_mat[2][2] = 1;
	new_t_mat[3][3] = 1;
	new_t_mat[0][3] = t[0];
	new_t_mat[1][3] = t[1];
	new_t_mat[2][3] = t[2];
	matrix44_multiply_inplace(new_t_mat, data->cam.t_mat);
	copy_r_mat(data);
	ft_memcpy(data->cam.origin, data->cam.origin_backup, sizeof(float [3]));
	matrix34_vec3_multiply_inplace(data->cam.t_mat, data->cam.origin);
	calculate_img(data);
}

void	rotate_cam(t_data *data, float theta, float axis[3], int anti_fa)
{
	double	new_t_mat[4][4];

	data->anti_fa = anti_fa;
	if (data->cam.mode)
		rodrigues_matrix_handler(axis, theta, data->cam.origin, new_t_mat);
	else
		rodrigues_matrix_handler(axis, theta, data->cam.world_center,
			new_t_mat);
	matrix44_multiply_inplace(new_t_mat, data->cam.t_mat);
	copy_r_mat(data);
	ft_memcpy(data->cam.origin, data->cam.origin_backup, sizeof(float [3]));
	matrix34_vec3_multiply_inplace(data->cam.t_mat, data->cam.origin);
	ft_memcpy(data->cam.x, (float [3]){1, 0, 0}, sizeof(float [3]));
	ft_memcpy(data->cam.y, (float [3]){0, 1, 0}, sizeof(float [3]));
	ft_memcpy(data->cam.z, (float [3]){0, 0, 1}, sizeof(float [3]));
	matrix33_vec3_multiply_inplace(data->cam.r_mat, data->cam.x);
	matrix33_vec3_multiply_inplace(data->cam.r_mat, data->cam.y);
	matrix33_vec3_multiply_inplace(data->cam.r_mat, data->cam.z);
	calculate_img(data);
}
