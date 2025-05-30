/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:22:11 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 15:25:19 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Implements BVH traversal using SIMD-accelerated AABB tests for visibility  */
/* and shadows. Provides both strict (sorted by distance) and fast            */
/* (early-exit) traversal modes. `visibility_test_bvh_strict` finds the       */
/* closest hit, while `visibility_test_bvh_fast` exits on first hit.          */
/* `shadow_test_bvh` checks for occlusion between a point and a light source. */
/* Utilizes SIMD to test 8 AABBs in parallel.                                 */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static float	process_bvh_children(t_bvh *bvh, int idx, t_shoot *shoot,
					float res[8]);

float	visibility_test_bvh_strict(t_bvh *bvh, int idx, t_shoot *shoot)
{
	__m256	v_res;
	float	res[8];

	if (bvh->children[idx] == -1)
		return (visi_test_leaves(bvh->group[idx], shoot, bvh->group_size[idx]));
	v_res = aabb_test_simd(bvh, bvh->children[idx], shoot->dir, shoot->src);
	_mm256_storeu_ps(res, v_res);
	return (process_bvh_children(bvh, idx, shoot, res));
}

static float	process_bvh_children(t_bvh *bvh, int idx, t_shoot *shoot,
	float res[8])
{
	float		t_min;
	float		t;
	int			i;
	t_object	*obj;

	t_min = FLT_MAX;
	obj = NULL;
	i = -1;
	while (++i < 8)
	{
		if (res[i] != -1)
		{
			t = visibility_test_bvh_strict(bvh, bvh->children[idx] + i, shoot);
			if (t > 0 && t < t_min)
			{
				t_min = t;
				obj = shoot->obj;
			}
		}
	}
	if (t_min != FLT_MAX)
	{
		shoot->obj = obj;
		return (t_min);
	}
	return (0);
}

float	visibility_test_bvh_fast(t_bvh *bvh, int idx, t_shoot *shoot)
{
	char		res[9];
	int			i;
	float		t;

	if (bvh->children[idx] == -1)
		return (visi_test_leaves(bvh->group[idx], shoot, bvh->group_size[idx]));
	aabb_test_fast(bvh, bvh->children[idx], shoot, res);
	i = -1;
	while (res[++i] != -1)
	{
		t = visibility_test_bvh_fast(bvh, bvh->children[idx] + res[i], shoot);
		if (t > 0)
			return (t);
	}
	return (0);
}

float	shadow_test_bvh(t_shoot *shoot, t_bvh *bvh, int idx, float dist_light)
{
	__m256		v_res;
	float		res[8];
	int			i;
	float		t;

	if (bvh->children[idx] == -1)
		return (shadow_intersect_leaves(shoot, bvh->group[idx], dist_light,
				bvh->group_size[idx]));
	v_res = aabb_test_simd(bvh, bvh->children[idx], shoot->shadow_ray,
			shoot->hit_pt);
	_mm256_storeu_ps(res, v_res);
	i = -1;
	while (++i < 8)
	{
		if (res[i] != -1)
		{
			t = shadow_test_bvh(shoot, bvh, bvh->children[idx] + i, dist_light);
			if (t > 0)
				return (t);
		}
	}
	return (0);
}
