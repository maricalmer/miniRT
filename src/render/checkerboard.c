/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:38:28 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 17:02:09 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Determines the checkerboard color at the hit point: returns 1 for color1   */
/* (e.g., black), 0 for color2 (e.g., white). Computes which grid cell the    */
/* point lies in using u/v coords then checks if the sum of cell indices is   */
/* odd or even. An odd sum means the cell is of the alternate color (1),      */
/* an even sum means it's the base color (0).                                 */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	eval_checkerboard_pattern(t_shoot *shoot)
{
	float	u;
	float	v;
	int		u_idx;
	int		v_idx;
	float	oc[3];

	vec_subtract(shoot->hit_pt, shoot->obj->geo.pl.point, oc);
	u = dot_vec3(oc, shoot->obj->geo.pl.u);
	v = dot_vec3(oc, shoot->obj->geo.pl.v);
	if (u < 0)
		u_idx = u * shoot->obj->mat.checker_size - 1;
	else
		u_idx = u * shoot->obj->mat.checker_size;
	if (v < 0)
		v_idx = v * shoot->obj->mat.checker_size - 1;
	else
		v_idx = v * shoot->obj->mat.checker_size;
	if ((u_idx + v_idx) & 1)
		return (1);
	return (0);
}
