/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:21:40 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 16:02:22 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Performs a ray-sphere intersection test. Calculates the intersection of a  */
/* ray with a sphere based on the ray's direction and origin. If an           */
/* intersection exists, it returns the distance to the point of intersection  */
/* and 0 otherwise.                                                           */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	calculate_sphere_coeffs(t_object *obj, float ray[3],
				float origin[3], float coeffs[3]);

float	intersect_sphere(t_object *obj, float ray[3], float origin[3])
{
	float	coeffs[3];
	float	discriminant;
	float	roots[2];

	calculate_sphere_coeffs(obj, ray, origin, coeffs);
	discriminant = coeffs[1] * coeffs[1] - 4 * coeffs[0] * coeffs[2];
	if (discriminant < 0)
		return (0);
	discriminant = sqrtf(discriminant);
	roots[0] = (-coeffs[1] - discriminant) / (2 * coeffs[0]);
	roots[1] = (-coeffs[1] + discriminant) / (2 * coeffs[0]);
	if (roots[0] > EPSILON)
		return (roots[0]);
	if (roots[1] > EPSILON)
		return (roots[1]);
	return (0);
}

static void	calculate_sphere_coeffs(t_object *obj, float ray[3],
			float origin[3], float coeffs[3])
{
	float	c_minus_o[3];

	vec_subtract(obj->geo.sph.center, origin, c_minus_o);
	coeffs[0] = dot_vec3(ray, ray);
	coeffs[1] = -2 * dot_vec3(ray, c_minus_o);
	coeffs[2] = dot_vec3(c_minus_o, c_minus_o) - obj->geo.sph.radius
		* obj->geo.sph.radius;
}
