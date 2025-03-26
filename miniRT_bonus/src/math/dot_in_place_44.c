/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dot_in_place_44.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:58:58 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/24 16:02:01 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	save_result_in_place_44(double res[4][4], double b[4][4]);

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
