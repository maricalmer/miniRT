/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_test_shadow.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:29:38 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 15:03:58 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Handles shadow ray tests against all scene objects. Offsets hit point      */
/* slightly to avoid self-shadowing artifacts. Supports direct object lists   */
/* and BVH-accelerated structures.                                            */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	shadow_intersect_objects(t_shoot *shoot, t_object *objects, float dist_light,
	int n_obj)
{
	float	t;
	int		i;
	float	hit_pt[3];

	hit_pt[0] = shoot->hit_pt[0] + EPSILON * shoot->normal[0];
	hit_pt[1] = shoot->hit_pt[1] + EPSILON * shoot->normal[1];
	hit_pt[2] = shoot->hit_pt[2] + EPSILON * shoot->normal[2];
	i = 0;
	while (i < n_obj)
	{
		if (objects[i].type == PLANE)
			t = intersect_plane(&objects[i], shoot->shadow_ray, hit_pt);
		else if (objects[i].type == CYLINDER)
			t = intersect_cylinder(&objects[i], shoot->shadow_ray, hit_pt);
		else
			t = shadow_test_bvh(shoot, objects[i].geo.bvh, 0, dist_light);
		if (t > 0 && dist_light > t)
			return (t);
		i++;
	}
	return (0);
}

float	shadow_intersect_leaves(t_shoot *shoot, t_object **objects, float dist_light,
	int n_obj)
{
	float	t;
	int		i;
	float	hit_pt[3];

	hit_pt[0] = shoot->hit_pt[0] + EPSILON * shoot->normal[0];
	hit_pt[1] = shoot->hit_pt[1] + EPSILON * shoot->normal[1];
	hit_pt[2] = shoot->hit_pt[2] + EPSILON * shoot->normal[2];
	i = 0;
	while (i < n_obj)
	{
		if (objects[i]->type == SPHERE)
			t = intersect_sphere(objects[i], shoot->shadow_ray, hit_pt);
		else if (objects[i]->type == TRI)
			t = intersect_triangle(objects[i], shoot->shadow_ray, hit_pt);
		else
			t = intersect_rectangle(objects[i], shoot->shadow_ray, hit_pt);
		if (t > 0 && dist_light > t)
			return (t);
		i++;
	}
	return (0);
}
