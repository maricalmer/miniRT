/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 20:51:30 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/20 21:21:47 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	normalize(float vector[3], float *magnitude)
{
	float	temp;
	float	mag;

	mag = sqrtf(vector[0] * vector[0]
			+ vector[1] * vector[1]
			+ vector[2] * vector[2]);
	if (magnitude)
		*magnitude = mag; 
	temp = 1 / mag;
	vector[0] *= temp;
	vector[1] *= temp;
	vector[2] *= temp;
}

float	dot_13_13(float a[3], float b[3])
{
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}

void	vec_substr(float p2[3], float p1[3], float result[3])
{
	int	i;

	i = -1;
	while (++i < 3)
		result[i] = p2[i] - p1[i];
}

void	cprod_13_13(float a[3], float b[3], float res[3])
{
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

float	triple_scalar(float a[3], float b[3], float c[3])
{
	return (a[0] * (b[1] * c[2] - b[2] * c[1])
		+ a[1] * (b[2] * c[0] - b[0] * c[2])
		+ a[2] * (b[0] * c[1] - b[1] * c[0]));
}
