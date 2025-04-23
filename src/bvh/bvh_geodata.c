/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_geodata.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:57:31 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/23 11:41:02 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* File extracts geo data (center, AABB min/max) for BVH-compatible objects:  */
/* spheres, triangles, and rectangles. Each object type has a specific        */
/* function to compute its axis-aligned bounding box and center position.     */
/* The main function allocates and fills an array of t_obj_geo for all        */
/* objects in the BVH root group. The extracted geometry data is stored in    */
/* the BVH structure and used later for spatial partitioning.                 */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static t_obj_geo	extract_geo_data_tri(t_object *obj);
static t_obj_geo	extract_geo_data_sphere(t_object *obj);
static t_obj_geo	extract_geo_data_rec(t_object *obj);

t_obj_geo	*create_obj_geo_data(t_bvh *bvh)
{
	t_object	*obj;
	t_obj_geo	*geo_data;
	int			i;

	geo_data = malloc(bvh->group_size[0] * sizeof(t_obj_geo));
	if (!geo_data)
		handle_memory_failure(__func__);
	bvh->obj_geo[0] = malloc(sizeof(t_obj_geo *) * bvh->group_size[0]);
	if (!bvh->obj_geo[0])
	{
		free(geo_data);
		handle_memory_failure(__func__);
	}
	i = -1;
	while (++i < bvh->group_size[0])
	{
		obj = bvh->group[0][i];
		if (obj->type == SPHERE)
			geo_data[i] = extract_geo_data_sphere(obj);
		if (obj->type == TRI)
			geo_data[i] = extract_geo_data_tri(obj);
		if (obj->type == RECTANGLE)
			geo_data[i] = extract_geo_data_rec(obj);
		bvh->obj_geo[0][i] = &geo_data[i];
	}
	return (geo_data);
}

static t_obj_geo	extract_geo_data_sphere(t_object *obj)
{
	t_obj_geo	geo;
	int			i;

	i = -1;
	while (++i < 3)
	{
		geo.center[i] = obj->geo.sph.center[i];
		geo.bmin[i] = obj->geo.sph.center[i] - obj->geo.sph.radius;
		geo.bmax[i] = obj->geo.sph.center[i] + obj->geo.sph.radius;
	}
	return (geo);
}

static t_obj_geo	extract_geo_data_tri(t_object *obj)
{
	t_obj_geo	geo;
	int			i;

	i = -1;
	while (++i < 3)
	{
		geo.center[i] = (obj->geo.tri.v0[i] + obj->geo.tri.v1[i]
				+ obj->geo.tri.v2[i]) / 3;
		geo.bmin[i] = fmin(obj->geo.tri.v0[i],
				fmin(obj->geo.tri.v1[i], obj->geo.tri.v2[i]));
		geo.bmax[i] = fmax(obj->geo.tri.v0[i],
				fmax(obj->geo.tri.v1[i], obj->geo.tri.v2[i]));
	}
	return (geo);
}

static t_obj_geo	extract_geo_data_rec(t_object *obj)
{
	t_obj_geo	geo;
	float		pts[4][3];
	int			i;

	i = -1;
	while (++i < 3)
	{
		pts[0][i] = obj->geo.rec.point[i] + obj->geo.rec.u[i] 
			* obj->geo.rec.uv_size + obj->geo.rec.v[i] * obj->geo.rec.uv_size;
		pts[1][i] = obj->geo.rec.point[i] - obj->geo.rec.u[i] 
			* obj->geo.rec.uv_size + obj->geo.rec.v[i] * obj->geo.rec.uv_size;
		pts[2][i] = obj->geo.rec.point[i] + obj->geo.rec.u[i] 
			* obj->geo.rec.uv_size - obj->geo.rec.v[i] * obj->geo.rec.uv_size;
		pts[3][i] = obj->geo.rec.point[i] - obj->geo.rec.u[i] 
			* obj->geo.rec.uv_size - obj->geo.rec.v[i] * obj->geo.rec.uv_size;
		geo.bmin[i] = fmin(fmin(fmin(pts[0][i], pts[1][i]),
					pts[2][i]), pts[3][i]);
		geo.bmax[i] = fmax(fmax(fmax(pts[0][i], pts[1][i]),
					pts[2][i]), pts[3][i]);
		geo.center[i] = obj->geo.rec.point[i];
	}
	return (geo);
}
