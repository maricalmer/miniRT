/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dot_in_place.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:34:09 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/23 16:36:21 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	save_result_in_place_13(float res[3], float b[3]);
static void	save_result_in_place_44(double res[4][4], double b[4][4]);

void	dot_inplace_34_13(double a[3][4], float b[3])
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
			res[i] += a[i][k] * b[k];
		res[i] += a[i][3] * 1;
	}
	save_result_in_place_13(res, b);
}

void	dot_inplace_33_13(float a[3][3], float b[3])
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
			res[i] += a[i][k] * b[k];
	}
	save_result_in_place_13(res, b);
}

void	dot_inplace_44_44(double a[4][4], double b[4][4])
{
	int		i;
	int		j;
	int		k;
	double	res[4][4];

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			res[i][j] = 0;
			k = -1;
			while (++k < 4)
				res[i][j] += a[i][k] * b[k][j];
		}
	}
	save_result_in_place_44(res, b);
}

static void	save_result_in_place_13(float res[3], float b[3])
{
	int	i;

	i = -1;
	while (++i < 3)
		b[i] = res[i];
}

static void	save_result_in_place_44(double res[4][4], double b[4][4])
{
	int	i;
	int	j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			b[i][j] = res[i][j];
	}
}

static void	save_result_in_place_33(float res[3][3], float b[3][3])
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

void	dot_inplace_33_33(float a[3][3], float b[3][3])
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
			{
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	save_result_in_place_33(res, a);
}
