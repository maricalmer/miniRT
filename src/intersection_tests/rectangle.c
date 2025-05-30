/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rectangle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:55:06 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 16:02:19 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Performs a ray-rectangle intersection test. Given a rectangle object and a */
/* ray, it calculates the intersection point and checks if it lies within the */
/* rectangle's bounds. Returns the distance to the rectangle if an            */
/* intersection exists and 0 otherwise.                                       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	intersect_rectangle(t_object *obj, float ray[3], float origin[3])
{
	float	delta_pos[3];
	float	a;
	float	b;
	float	hit_pt[3];
	float	uv[2];

	a = dot_vec3(obj->geo.rec.normal, ray);
	if (a < EPSILON && a > -EPSILON)
		return (0);
	vec_subtract(obj->geo.rec.point, origin, delta_pos);
	b = dot_vec3(obj->geo.rec.normal, delta_pos);
	if ((a < 0 && b > 0) || (a > 0 && b < 0))
		return (0);
	a = b / a;
	hit_pt[0] = origin[0] + a * ray[0];
	hit_pt[1] = origin[1] + a * ray[1];
	hit_pt[2] = origin[2] + a * ray[2];
	vec_subtract(hit_pt, obj->geo.rec.point, delta_pos);
	uv[0] = dot_vec3(delta_pos, obj->geo.rec.u);
	uv[1] = dot_vec3(delta_pos, obj->geo.rec.v);
	if (uv[0] < -obj->geo.rec.uv_size || uv[0] > obj->geo.rec.uv_size
		|| uv[1] < -obj->geo.rec.uv_size || uv[1] > obj->geo.rec.uv_size)
		return (0);
	return (a);
}
