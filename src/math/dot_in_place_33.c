/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dot_in_place_33.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:34:09 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 15:42:18 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Provides in-place matrix-vector and matrix-matrix multiplication routines. */
/* Supports 3x3 and 3x4 matrix operations with 3D vectors or matrices,        */
/* storing the result directly back into the input.                           */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	copy_vec3_inplace(float res[3], float b[3]);
static void	copy_matrix3x3_inplace(float res[3][3], float b[3][3]);

void	matrix34_vec3_multiply_inplace(double matrix[3][4], float vector[3])
{
	int		i;
	int		k;
	float	res[3];

	i = -1;
	while (++i < 3)
	{
		res[i] = 0;
		k = -1;
		while (++k < 3)
			res[i] += matrix[i][k] * vector[k];
		res[i] += matrix[i][3] * 1;
	}
	copy_vec3_inplace(res, vector);
}

void	matrix33_vec3_multiply_inplace(float matrix[3][3], float vector[3])
{
	int		i;
	int		k;
	float	res[3];

	i = -1;
	while (++i < 3)
	{
		res[i] = 0;
		k = -1;
		while (++k < 3)
			res[i] += matrix[i][k] * vector[k];
	}
	copy_vec3_inplace(res, vector);
}

void	matrix33_matrix33_multiply_inplace(float a[3][3], float b[3][3])
{
	int		i;
	int		j;
	int		k;
	float	res[3][3];

	i = -1;
	while (++i < 3)
	{
		j = -1;
		while (++j < 3)
		{
			res[i][j] = 0;
			k = -1;
			while (++k < 3)
				res[i][j] += a[i][k] * b[k][j];
		}
	}
	copy_matrix3x3_inplace(res, a);
}

static void	copy_vec3_inplace(float res[3], float b[3])
{
	int	i;

	i = -1;
	while (++i < 3)
		b[i] = res[i];
}

static void	copy_matrix3x3_inplace(float res[3][3], float b[3][3])
{
	int	i;
	int	j;

	i = -1;
	while (++i < 3)
	{
		j = -1;
		while (++j < 3)
			b[i][j] = res[i][j];
	}
}
