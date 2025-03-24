/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rodrigues.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 21:34:47 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/24 16:01:42 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	is_special_case(double t_mat[4][4], float cam_dir[3]);
static void	get_rodrigues_matrice(float u[3], float cos_sin_thetas[2],
				float c[3], double r[4][4]);

void	get_rotation_matrice(float cam_dir[3], double t_mat[4][4], float c[3])
{
	float	v1[3];
	float	u[3];
	float	cos_sin_thetas[2];

	v1[0] = 0;
	v1[1] = 0;
	v1[2] = -1;
	if (is_special_case(t_mat, cam_dir))
		return ;
	cprod_13_13(v1, cam_dir, u);
	normalize(u, &cos_sin_thetas[1]);
	cos_sin_thetas[0] = dot_13_13(v1, cam_dir);
	get_rodrigues_matrice(u, cos_sin_thetas, c, t_mat);
}

static int	is_special_case(double t_mat[4][4], float cam_dir[3])
{
	if (cam_dir[0] == 0 && cam_dir[1] == 0 && cam_dir[2] == -1)
	{
		ft_memset(t_mat, 0, sizeof(double [4][4]));
		t_mat[0][0] = 1;
		t_mat[1][1] = 1;
		t_mat[2][2] = 1;
		t_mat[3][3] = 1;
		return (1);
	}
	if (cam_dir[0] == 0 && cam_dir[1] == 0 && cam_dir[2] == 1)
	{
		ft_memset(t_mat, 0, sizeof(double [4][4]));
		t_mat[0][0] = -1;
		t_mat[1][1] = 1;
		t_mat[2][2] = -1;
		t_mat[3][3] = 1;
		return (1);
	}
	return (0);
}

void	rodrigues_matrice_handler(float u[3], float theta, float c[3],
	double r[4][4])
{
	float	cos_sin_thetas[2];

	cos_sin_thetas[0] = cos(theta);
	cos_sin_thetas[1] = sin(theta);
	get_rodrigues_matrice(u, cos_sin_thetas, c, r);
}

static void	get_rodrigues_matrice(float u[3], float cos_sin_thetas[2],
	float c[3], double r[4][4])
{
	r[0][0] = cos_sin_thetas[0] + u[0] * u[0] * (1 - cos_sin_thetas[0]);
	r[0][1] = u[0] * u[1] * (1 - cos_sin_thetas[0]) - u[2] * cos_sin_thetas[1];
	r[0][2] = u[0] * u[2] * (1 - cos_sin_thetas[0]) + u[1] * cos_sin_thetas[1];
	r[1][0] = u[0] * u[1] * (1 - cos_sin_thetas[0]) + u[2] * cos_sin_thetas[1];
	r[1][1] = cos_sin_thetas[0] + u[1] * u[1] * (1 - cos_sin_thetas[0]);
	r[1][2] = u[1] * u[2] * (1 - cos_sin_thetas[0]) - u[0] * cos_sin_thetas[1];
	r[2][0] = u[2] * u[0] * (1 - cos_sin_thetas[0]) - u[1] * cos_sin_thetas[1];
	r[2][1] = u[2] * u[1] * (1 - cos_sin_thetas[0]) + u[0] * cos_sin_thetas[1];
	r[2][2] = cos_sin_thetas[0] + u[2] * u[2] * (1 - cos_sin_thetas[0]);
	r[0][3] = c[0] * (1 - r[0][0]) - c[1] * r[0][1] - c[2] * r[0][2];
	r[1][3] = -c[0] * r[1][0] + c[1] * (1 - r[1][1]) - c[2] * r[1][2];
	r[2][3] = -c[0] * r[2][0] - c[1] * r[2][1] + c[2] * (1 - r[2][2]);
	ft_memset(r[3], 0, sizeof(double [3]));
	r[3][3] = 1;
}

void	first_rotation_matrice(t_data *data)
{
	float	tmp[3];

	get_rotation_matrice(data->cam.direction, data->cam.t_mat,
		(float [3]){0, 0, 0});
	cpy_vec((float [3]){0, 1, 0}, tmp);
	copy_r_mat_0(data);
	ft_memset(data->cam.t_mat, 0, sizeof(double [4][4]));
	data->cam.t_mat[0][0] = 1;
	data->cam.t_mat[1][1] = 1;
	data->cam.t_mat[2][2] = 1;
	data->cam.t_mat[3][3] = 1;
	ft_memcpy(data->cam.x, (float [3]){1, 0, 0}, sizeof(float [3]));
	ft_memcpy(data->cam.y, (float [3]){0, 1, 0}, sizeof(float [3]));
	ft_memcpy(data->cam.z, (float [3]){0, 0, 1}, sizeof(float [3]));
	dot_inplace_33_13(data->cam.r_mat, data->cam.x);
	dot_inplace_33_13(data->cam.r_mat, data->cam.y);
	dot_inplace_33_13(data->cam.r_mat, data->cam.z);
}
