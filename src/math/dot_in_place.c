/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dot_in_place.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:34:09 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/20 19:20:16 by dlemaire         ###   ########.fr       */
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
	save_result_in_place_44(res, a);
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
