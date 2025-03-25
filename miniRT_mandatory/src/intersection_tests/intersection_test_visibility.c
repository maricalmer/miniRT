/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_test_visibility.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:52:02 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/25 11:48:28 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static float	find_closest_hit(t_object *objects, t_shoot *shoot, int n_obj);
static float	test_object(t_object *obj, t_shoot *shoot);

float	visi_tests(t_object *objects, t_shoot *shoot, int n_obj)
{
	float	t_min;

	t_min = find_closest_hit(objects, shoot, n_obj);
	if (t_min != FLT_MAX)
		return (t_min);
	else
		return (0);
}

static float	find_closest_hit(t_object *objects, t_shoot *shoot, int n_obj)
{
	float	t_min;
	float	t;
	int		i;

	t_min = FLT_MAX;
	i = -1;
	while (++i < n_obj)
	{
		t = test_object(&objects[i], shoot);
		if (t > EPSILON && t < t_min)
		{
			t_min = t;
			if (objects[i].type != BVH)
				shoot->obj = &objects[i];
		}
	}
	return (t_min);
}

static float	test_object(t_object *obj, t_shoot *shoot)
{
	if (obj->type == PLANE)
		return (test_plane(obj, shoot->dir, shoot->src));
	else if (obj->type == CYLINDER)
		return (test_cylinder(obj, shoot->dir, shoot->src));
	else if (obj->type == SPHERE)
		return (test_sphere(obj, shoot->dir, shoot->src));
	return (0);
}
