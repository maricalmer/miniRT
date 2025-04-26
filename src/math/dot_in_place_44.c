/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dot_in_place_44.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:58:58 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 15:46:58 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Performs in-place multiplication of two 4x4 matrices. The result           */
/* overwrites the second input matrix to avoid extra memory allocation.       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	copy_matrix44_inplace(double res[4][4], double b[4][4]);

void	matrix44_multiply_inplace(double a[4][4], double b[4][4])
{
	int		i;
	int		j;
	int		k;
	double	result[4][4];

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			result[i][j] = 0;
			k = -1;
			while (++k < 4)
				result[i][j] += a[i][k] * b[k][j];
		}
	}
	copy_matrix44_inplace(result, b);
}

static void	copy_matrix44_inplace(double result[4][4], double b[4][4])
{
	int	i;
	int	j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			b[i][j] = result[i][j];
	}
}
