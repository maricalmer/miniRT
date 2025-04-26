/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_bbox.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:52:57 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 13:17:05 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Computes and stores bounding boxes (AABBs) for nodes and elements (tri,    */
/* sphere...) in the BVH. Boxes are split between left and right partitions   */
/* along a selected axis which is set to tightly fit all geometry elements.   */
/* The final bounding box is the intersection of a node's spatial region and  */
/* the bounds of the geometry elements it contains. These bounds are stored   */
/* back into BVH structure for further processing.                            */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static t_bbox	get_bbox_half(t_bvh *bvh, int idx, t_cut_in_two *cut, int side);
static t_bbox	get_bbox_group(t_bvh *bvh, int idx);
static void		save_bbox_overlap(t_bvh *bvh, t_bbox bbox_node,
					t_bbox bbox_elem, int idx);

void	get_bboxes(t_bvh *bvh, int idx, t_cut_in_two *cut)
{
	t_bbox	bbox_new_node;
	t_bbox	bbox_elem;

	bbox_new_node = get_bbox_half(bvh, idx, cut, RIGHT_HALF);
	bbox_elem = get_bbox_group(bvh, cut->idx_right);
	save_bbox_overlap(bvh, bbox_new_node, bbox_elem, cut->idx_right);
	bbox_new_node = get_bbox_half(bvh, idx, cut, LEFT_HALF);
	bbox_elem = get_bbox_group(bvh, cut->idx_left);
	save_bbox_overlap(bvh, bbox_new_node, bbox_elem, cut->idx_left);
}

void	get_bbox_min_max_root(t_bvh *bvh)
{
	t_bbox	bbox1;
	t_bbox	bbox2;

	bbox1 = get_bbox_group(bvh, 0);
	bbox2 = bbox1;
	save_bbox_overlap(bvh, bbox1, bbox2, 0);
}

static t_bbox	get_bbox_half(t_bvh *bvh, int idx, t_cut_in_two *cut, int side)
{
	t_bbox	res;

	res.min[0] = bvh->min_x[idx];
	res.min[1] = bvh->min_y[idx];
	res.min[2] = bvh->min_z[idx];
	res.max[0] = bvh->max_x[idx];
	res.max[1] = bvh->max_y[idx];
	res.max[2] = bvh->max_z[idx];
	if (side == LEFT_HALF)
		res.max[cut->axis] = cut->mid[cut->axis];
	else if (side == RIGHT_HALF)
		res.min[cut->axis] = cut->mid[cut->axis];
	return (res);
}

static t_bbox	get_bbox_group(t_bvh *bvh, int idx)
{
	t_bbox	res;
	int		i;
	int		j;
	float	min;
	float	max;

	i = -1;
	while (++i < 3)
	{
		min = FLT_MAX;
		max = -FLT_MAX;
		j = -1;
		while (++j < bvh->group_size[idx])
		{
			if (bvh->obj_geo[idx][j]->bmin[i] < min)
				min = bvh->obj_geo[idx][j]->bmin[i];
			if (bvh->obj_geo[idx][j]->bmax[i] > max)
				max = bvh->obj_geo[idx][j]->bmax[i]; 
		}
		res.min[i] = min;
		res.max[i] = max;
	}
	return (res);
}

static void	save_bbox_overlap(t_bvh *bvh, t_bbox bbox_node,
	t_bbox bbox_elem, int idx)
{
	bvh->min_x[idx] = fmax(bbox_node.min[0], bbox_elem.min[0]);
	bvh->min_y[idx] = fmax(bbox_node.min[1], bbox_elem.min[1]);
	bvh->min_z[idx] = fmax(bbox_node.min[2], bbox_elem.min[2]);
	bvh->max_x[idx] = fmin(bbox_node.max[0], bbox_elem.max[0]);
	bvh->max_y[idx] = fmin(bbox_node.max[1], bbox_elem.max[1]);
	bvh->max_z[idx] = fmin(bbox_node.max[2], bbox_elem.max[2]);
}

