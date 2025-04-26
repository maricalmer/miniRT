/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_in_two.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:28:39 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 13:58:27 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Subdivides a BVH node into two child nodes recursively. From depth 1       */
/* onward, cutting occurs in-place within the preallocated SoA BVH structure. */
/* The axis is chosen by evaluating object counts across mid-planes (up to    */
/* depth 3, splits tend to happen along the Y axis). The node is split into   */
/* left/right groups based on AABB bounds. Child groups are allocated and     */
/* filled with relevant objects and geometry data. AABBs are computed for the */
/* children, and memory is cleaned up when needed.                            */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	choose_split_axis_and_counts(t_bvh *bvh, int idx,
				t_cut_in_two *cut);
static void	create_child_groups(t_bvh *bvh, t_cut_in_two *cut);

void	cut_in_two(t_bvh *bvh, int idx, int idx_child, int cut_lvl)
{
	t_cut_in_two	cut;

	if (cut_lvl == -1)
		return ;
	if (cut_lvl == 2)
		cut.idx_left = idx_child;
	else
		cut.idx_left = idx;
	cut.idx_right = cut.idx_left + pow(2, cut_lvl);
	cut.old_n_obj = bvh->group_size[idx];
	cut.old_group = bvh->group[idx];
	cut.old_geo = bvh->obj_geo[idx];
	get_mid_planes(bvh, idx, &cut);
	choose_split_axis_and_counts(bvh, idx, &cut);
	malloc_groups_and_geo(bvh, &cut);
	create_child_groups(bvh, &cut);
	get_bboxes(bvh, idx, &cut);
	if (idx == cut.idx_left)
	{
		free(cut.old_group);
		free(cut.old_geo);
	}
	cut_in_two(bvh, cut.idx_left, idx_child, cut_lvl - 1);
	cut_in_two(bvh, cut.idx_right, idx_child, cut_lvl - 1);
}

static void	choose_split_axis_and_counts(t_bvh *bvh, int idx, t_cut_in_two *cut)
{
	int		i;
	int		j;
	int		n_obj_l[3];
	int		n_obj_r[3];
	int		sum[3];

	ft_memset(n_obj_l, 0, sizeof(int [3]));
	ft_memset(n_obj_r, 0, sizeof(int [3]));
	j = -1;
	while (++j < 3)
	{
		i = -1;
		while (++i < cut->old_n_obj) 
		{
			if (bvh->obj_geo[idx][i]->bmin[j] <= cut->mid[j])
				n_obj_l[j]++;
			if (bvh->obj_geo[idx][i]->bmax[j] >= cut->mid[j])
				n_obj_r[j]++;
		}
		sum[j] = n_obj_l[j] + n_obj_r[j];
	}
	cut->axis = find_min_idx(sum);
	bvh->group_size[cut->idx_left] = n_obj_l[cut->axis];
	bvh->group_size[cut->idx_right] = n_obj_r[cut->axis];
}

static void	create_child_groups(t_bvh *bvh, t_cut_in_two *cut)
{
	int		i;
	int		i_left;
	int		i_right;

	i = -1;
	i_left = 0;
	i_right = 0;
	while (++i < cut->old_n_obj) 
	{
		if (cut->old_geo[i]->bmin[cut->axis] <= cut->mid[cut->axis])
		{
			bvh->group[cut->idx_left][i_left] = cut->old_group[i];
			bvh->obj_geo[cut->idx_left][i_left++] = cut->old_geo[i];
		}
		if (cut->old_geo[i]->bmax[cut->axis] >= cut->mid[cut->axis])
		{
			bvh->group[cut->idx_right][i_right] = cut->old_group[i];
			bvh->obj_geo[cut->idx_right][i_right++] = cut->old_geo[i];
		}
	}
}
