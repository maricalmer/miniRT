/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:57:55 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 16:02:08 by dlemaire         ###   ########.fr       */
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
static int	compute_intersections(float coeffs[3], float roots[2]);
static int	check_cylinder_hit(t_object *obj, float hit_t, float ray[3],
				float origin[3]);

float	intersect_cylinder(t_object *obj, float ray[3], float origin[3])
{
	float	coeffs[3];
	float	roots[2];

	calculate_cylinder_coeffs(obj, ray, origin, coeffs);
	if (!compute_intersections(coeffs, roots))
		return (0);
	if (roots[0] < EPSILON)
		roots[0] = roots[1];
	if (check_cylinder_hit(obj, roots[0], ray, origin))
		return (roots[0]);
	if (check_cylinder_hit(obj, roots[1], ray, origin))
		return (roots[1]);
	return (0);
}

static void	calculate_cylinder_coeffs(t_object *obj, float ray[3],
	float origin[3], float coeffs[3])
{
	float	tmp[3];
	float	tmp2[3];
	float	r_ao[3];
	float	v_a[3];

	vec_subtract(origin, obj->geo.cyl.center, tmp);
	cross_vec3(tmp, obj->geo.cyl.dir, tmp2);
	cross_vec3(obj->geo.cyl.dir, tmp2, r_ao);
	cross_vec3(ray, obj->geo.cyl.dir, tmp);
	cross_vec3(obj->geo.cyl.dir, tmp, v_a);
	coeffs[0] = dot_vec3(v_a, v_a);
	coeffs[1] = 2 * dot_vec3(r_ao, v_a);
	coeffs[2] = dot_vec3(r_ao, r_ao) - obj->geo.cyl.radius
		* obj->geo.cyl.radius;
}

static int	compute_intersections(float coeffs[3], float roots[2])
{
	float	discriminant;

	discriminant = coeffs[1] * coeffs[1] - 4 * coeffs[0] * coeffs[2];
	if (discriminant < 0)
		return (0);
	discriminant = sqrtf(discriminant);
	roots[0] = (-coeffs[1] - discriminant) / (2 * coeffs[0]);
	roots[1] = (-coeffs[1] + discriminant) / (2 * coeffs[0]);
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
	h = dot_vec3(tmp, obj->geo.cyl.dir);
	return (h >= -obj->geo.cyl.height * 0.5f && h <= obj->geo.cyl.height * 0.5f);
}
