/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_test_visibility.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:52:02 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 14:52:06 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Performs visibility tests for all scene objects. Supports primitive types  */
/* and BVH-accelerated geometry. Uses FAST_BVH flag to determine traversal    */
/* strategy (fast or strict).                                                 */
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
		return (intersect_cylinder(obj, shoot->dir, shoot->src));
	else if (obj->type == BVH)
	{
		if (FAST_BVH_TRANSVERSAL)
			return (visibility_test_bvh_fast(obj->geo.bvh, 0, shoot));
		else
			return (visibility_test_bvh_strict(obj->geo.bvh, 0, shoot));
	}
	return (0);
}

float	visi_test_leafs(t_object **objects, t_shoot *shoot, int n_obj)
{
	float		t_min;
	float		t;
	int			i;

	t_min = FLT_MAX;
	t = 0;
	i = -1;
	while (++i < n_obj)
	{
		if (objects[i]->type == SPHERE)
			t = test_sphere(objects[i], shoot->dir, shoot->src);
		else if (objects[i]->type == TRI)
			t = test_triangle(objects[i], shoot->dir, shoot->src);
		else
			t = test_rectangle(objects[i], shoot->dir, shoot->src);
		if (t > EPSILON && t < t_min)
		{
			t_min = t;
			shoot->obj = objects[i];
		}
	}
	if (t_min != FLT_MAX)
		return (t_min);
	return (0);
}
