/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 21:14:45 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/20 21:22:07 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	ft_swap(float *t1, float *t2)
{
	float	tmp;

	tmp = *t1;
	*t1 = *t2;
	*t2 = tmp;
}

int	imin(int a, int b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

int	imax(int a, int b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

void	cpy_vec(float v1[3], float v2[3])
{
	v2[0] = v1[0];
	v2[1] = v1[1];
	v2[2] = v1[2];
}

void	scale_vec(float v[3], float amp)
{
	v[0] *= amp;
	v[1] *= amp;
	v[2] *= amp;
}
