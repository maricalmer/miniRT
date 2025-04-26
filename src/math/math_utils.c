/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 21:14:45 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 15:55:15 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Utility functions for basic numerical and vector operations. Includes      */
/* float swapping, min/max comparison, vector copying, and uniform vector     */
/* scaling.                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	swap_float(float *t1, float *t2)
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

void	copy_vec(float src[3], float dst[3])
{
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

void	scale_vec(float v[3], float scalar)
{
	v[0] *= scalar;
	v[1] *= scalar;
	v[2] *= scalar;
}
