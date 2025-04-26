/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:55:06 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 15:01:47 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Performs a ray-plane intersection test. Given a plane object and a ray,    */
/* it calculates the intersection point, returning the distance to the plane  */
/* if an intersection exists and 0 otherwise.                                 */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	intersect_plane(t_object *obj, float ray[3], float origin[3])
{
	float	pts_d[3];
	float	a;
	float	b;

	a = dot_13_13(obj->geo.pl.normal, ray);
	if (a < EPSILON && a > -EPSILON)
		return (0);
	vec_substr (obj->geo.pl.point, origin, pts_d);
	b = dot_13_13(obj->geo.pl.normal, pts_d);
	if ((a < 0 && b > 0) || (a > 0 && b < 0))
		return (0);
	return (b / a);
}
