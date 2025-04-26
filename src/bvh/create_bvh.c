/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_bvh.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:01:41 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 15:27:20 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Initializes a BVH (Bounding Volume Hierarchy) structure from scene data.   */
/* It allocates and zeroes out the BVH’s root node and metadata.              */
/* Geometry-relevant objects are grouped and their spatial data extracted.    */
/* The root AABB is computed, then the BVH is recursively split into 8-wide   */
/* child nodes. Each node is subdivided or marked as a leaf, based on size    */
/* and depth constraints. Failed subdivisions are reverted.                   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	cut_into_child_nodes(t_bvh *bvh, int idx);
static int	handle_fail_to_cut(t_bvh *bvh, int idx);
static void	handle_recursion_children(t_bvh *bvh, int idx);
static void	memset_bvh8_first_block(t_bvh *bvh);

t_bvh	*init_bvh(t_data *data)
{
	t_bvh			*bvh;
	int				idx_child;
	struct timeval	t_start;
	struct timeval	t_end;

	gettimeofday(&t_start, NULL);
	bvh = aligned_alloc(ALGN_BVH_STR, sizeof(t_bvh));
	if (!bvh)
		handle_memory_failure(__func__);
	memset_bvh8_first_block(bvh);
	get_group_size(data, bvh);
	create_root_obj_list(data, bvh);
	data->bvh_geo_data = create_obj_geo_data(bvh);
	get_bbox_min_max_root(bvh);
	idx_child = cut_into_child_nodes(bvh, 0);
	bvh->children[idx_child] = BVH_CHILD_END;
	print_bvh_stats(bvh);
	free_bvh_nonleaf_and_geo(bvh);
	free(data->bvh_geo_data);
	remap_objects_after_bvh(data, bvh);
	gettimeofday(&t_end, NULL);
	print_bvh_build_time(t_start, t_end);
	return (bvh);
}

static void	memset_bvh8_first_block(t_bvh *bvh)
{
	ft_memset(bvh->min_x, 0, sizeof(float [8]));
	ft_memset(bvh->min_y, 0, sizeof(float [8]));
	ft_memset(bvh->min_z, 0, sizeof(float [8]));
	ft_memset(bvh->max_x, 0, sizeof(float [8]));
	ft_memset(bvh->max_y, 0, sizeof(float [8]));
	ft_memset(bvh->max_z, 0, sizeof(float [8]));
	ft_memset(bvh->group_size, 0, sizeof(int [8]));
	ft_memset(bvh->group, 0, sizeof(void *) * 8);
	ft_memset(bvh->children, 0, sizeof(int) * 8);
	ft_memset(bvh->depth, 0, sizeof(int) * 8);
	ft_memset(bvh->obj_geo, 0, sizeof(void *) * 8);
}

static int	cut_into_child_nodes(t_bvh *bvh, int idx)
{
	static int	idx_child = 8;
	int			i;
	int			fail_to_cut;

	if (bvh->group_size[idx] <= MAX_BVH_GROUP
		|| bvh->depth[idx] == BVH_DEPTH_MAX)
	{
		bvh->children[idx] = BVH_LEAF;
		return (idx_child);
	}
	cut_in_two(bvh, idx, idx_child, 2);
	bvh->children[idx] = idx_child;
	i = -1;
	fail_to_cut = 0;
	while (++i < 8)
		if (bvh->group_size[bvh->children[idx] + i] == bvh->group_size[idx])
			fail_to_cut = 1;
	if (fail_to_cut)
		return (handle_fail_to_cut(bvh, idx));
	idx_child += 8;
	handle_recursion_children(bvh, idx);
	return (idx_child);
}

static int	handle_fail_to_cut(t_bvh *bvh, int idx)
{
	int	i;

	i = -1;
	while (++i < 8)
	{
		free(bvh->group[bvh->children[idx] + i]);
		free(bvh->obj_geo[bvh->children[idx] + i]);
	}
	bvh->children[idx] = BVH_LEAF;
	return (0);
}

static void	handle_recursion_children(t_bvh *bvh, int idx)
{
	int	i;

	i = -1;
	while (++i < 8)
	{
		bvh->depth[bvh->children[idx] + i] = bvh->depth[idx] + 1;
		cut_into_child_nodes(bvh, bvh->children[idx] + i);
	}
}
