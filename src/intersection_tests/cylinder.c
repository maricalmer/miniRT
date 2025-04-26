/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:57:55 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 14:57:33 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Implements ray-cylinder intersection for finite cylinders. Computes        */
/* quadratic intersection using cross products and dot products. Validates    */
/* hits based on the cylinder's finite height along its axis.                 */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	calculate_cylinder_coeffs(t_object *obj, float ray[3],
				float origin[3], float coeffs[3]);
static int	compute_intersections(float coeffs[3], float results[2]);
static int	check_cylinder_hit(t_object *obj, float hit_t, float ray[3],
				float origin[3]);

float	intersect_cylinder(t_object *obj, float ray[3], float origin[3])
{
	float	coeffs[3];
	float	results[2];

	calculate_cylinder_coeffs(obj, ray, origin, coeffs);
	if (!compute_intersections(coeffs, results))
		return (0);
	if (results[0] < EPSILON)
		results[0] = results[1];
	if (check_cylinder_hit(obj, results[0], ray, origin))
		return (results[0]);
	if (check_cylinder_hit(obj, results[1], ray, origin))
		return (results[1]);
	return (0);
}

static void	calculate_cylinder_coeffs(t_object *obj, float ray[3],
	float origin[3], float coeffs[3])
{
	float	tmp[3];
	float	tmp2[3];
	float	r_ao[3];
	float	v_a[3];

	vec_substr(origin, obj->geo.cyl.center, tmp);
	cprod_13_13(tmp, obj->geo.cyl.dir, tmp2);
	cprod_13_13(obj->geo.cyl.dir, tmp2, r_ao);
	cprod_13_13(ray, obj->geo.cyl.dir, tmp);
	cprod_13_13(obj->geo.cyl.dir, tmp, v_a);
	coeffs[0] = dot_13_13(v_a, v_a);
	coeffs[1] = 2 * dot_13_13(r_ao, v_a);
	coeffs[2] = dot_13_13(r_ao, r_ao) - obj->geo.cyl.radius
		* obj->geo.cyl.radius;
}

static int	compute_intersections(float coeffs[3], float results[2])
{
	float	discriminant;

	discriminant = coeffs[1] * coeffs[1] - 4 * coeffs[0] * coeffs[2];
	if (discriminant < 0)
		return (0);
	discriminant = sqrtf(discriminant);
	results[0] = (-coeffs[1] - discriminant) / (2 * coeffs[0]);
	results[1] = (-coeffs[1] + discriminant) / (2 * coeffs[0]);
	return (1);
}

static int	check_cylinder_hit(t_object *obj, float hit_t, float ray[3],
	float origin[3])
{
	float	tmp[3];
	int		i;
	float	h;

	i = -1;
	while (++i < 3)
		tmp[i] = origin[i] + hit_t * ray[i] - obj->geo.cyl.center[i];
	h = dot_13_13(tmp, obj->geo.cyl.dir);
	return (h >= -obj->geo.cyl.height * 0.5f && h <= obj->geo.cyl.height * 0.5f);
}
