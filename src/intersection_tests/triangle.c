/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:36:17 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 15:21:30 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Performs a ray-triangle intersection test using the Möller-Trumbore        */
/* algorithm. This test calculates whether a ray intersects a triangle in 3D  */
/* space, and if so, it returns the distance to the intersection point and 0  */
/* otherwise.                                                                 */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static float	compute_barycentric_t(float s[3], float ray[3], float edges[6],
					float det_and_bar[4]);

float	intersect_triangle(t_object *obj, float ray[3], float origin[3])
{
	float	edges[6];
	float	s[3];
	float	h[3];
	float	det_and_bar[4];

	vec_substr(obj->geo.tri.v1, obj->geo.tri.v0, edges);
	vec_substr(obj->geo.tri.v2, obj->geo.tri.v0, edges + 3);
	cprod_13_13(ray, edges + 3, h);
	det_and_bar[0] = dot_13_13(edges, h);
	if (fabs(det_and_bar[0]) < EPSILON)
		return (0.0f);
	vec_substr(origin, obj->geo.tri.v0, s);
	det_and_bar[1] = 1.0f / det_and_bar[0];
	det_and_bar[2] = dot_13_13(s, h) * det_and_bar[1];
	if (det_and_bar[2] < 0.0f || det_and_bar[2] > 1.0f)
		return (0.0f);
	return (compute_barycentric_t(s, ray, edges, det_and_bar));
}

static float	compute_barycentric_t(float s[3], float ray[3], float edges[6],
	float det_and_bar[4])
{
	float	q[3];
	float	t;

	cprod_13_13(s, edges, q);
	det_and_bar[3] = dot_13_13(ray, q) * det_and_bar[1];
	if (det_and_bar[3] < 0.0f || det_and_bar[2] + det_and_bar[3] > 1.0f)
		return (0.0f);
	t = dot_13_13(edges + 3, q) * det_and_bar[1];
	return (t);
}
