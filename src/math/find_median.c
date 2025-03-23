/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_median.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 22:39:15 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/22 21:26:44 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static float	quick_select(t_bvh *bvh, int bounds[2], int k,
					int idx_axis[2]);
static int		partition(t_bvh *bvh, int bounds[2], int idx_pivot,
					int idx_axis[2]);
void			ft_swap_custom(t_bvh *bvh, int idx_1, int idx_2, int node_idx);

float	find_median_custom(t_bvh *bvh, int idx, int axis)
{
	float	mid_l;
	float	mid_r;
	int		bounds[2];
	int		n;
	int		idx_axis[2];

	n = bvh->group_size[idx];
	bounds[0] = 0;
	bounds[1] = n - 1;
	idx_axis[0] = idx;
	idx_axis[1] = axis;
	if (n & 1)
		return (quick_select(bvh, bounds, n / 2, idx_axis));
	else
	{
		mid_l = quick_select(bvh, bounds, n / 2 - 1, idx_axis);
		mid_r = quick_select(bvh, bounds, n / 2, idx_axis);
		return ((mid_l + mid_r) * 0.5f);
	}
}

static float	quick_select(t_bvh *bvh, int bounds[2], int k,
	int idx_axis[2])
{
	int	idx_pivot;
	int	bounds_l[2];
	int	bounds_r[2];

	if (bounds[0] == bounds[1])
		return (bvh->obj_geo[idx_axis[0]][bounds[0]]->center[idx_axis[1]]);
	idx_pivot = bounds[0] + rand() % (bounds[1] - bounds[0] + 1);
	idx_pivot = partition(bvh, bounds, idx_pivot, idx_axis);
	if (k == idx_pivot)
		return (bvh->obj_geo[idx_axis[0]][k]->center[idx_axis[1]]);
	else if (k < idx_pivot && bounds[0] < idx_pivot - 1)
	{
		bounds_l[0] = bounds[0];
		bounds_l[1] = idx_pivot - 1;
		return (quick_select(bvh, bounds_l, k, idx_axis));
	}
	else if (k > idx_pivot && idx_pivot + 1 <= bounds[1])
	{
		bounds_r[0] = idx_pivot + 1;
		bounds_r[1] = bounds[1];
		return (quick_select(bvh, bounds_r, k, idx_axis));
	}
	return (bvh->obj_geo[idx_axis[0]][k]->center[idx_axis[1]]);
}

static int	partition(t_bvh *bvh, int bounds[2], int idx_pivot,
	int idx_axis[2])
{
	float	pivot;
	int		idx_final;
	int		i;

	pivot = bvh->obj_geo[idx_axis[0]][idx_pivot]->center[idx_axis[1]];
	ft_swap_custom(bvh, idx_pivot, bounds[1], idx_axis[0]);
	idx_final = bounds[0];
	i = bounds[0];
	while (i < bounds[1])
	{
		if (bvh->obj_geo[idx_axis[0]][i]->center[idx_axis[1]] < pivot)
		{
			ft_swap_custom(bvh, i, idx_final, idx_axis[0]);
			idx_final++;
		}
		i++;
	}
	ft_swap_custom(bvh, idx_final, bounds[1], idx_axis[0]);
	return (idx_final);
}

void	ft_swap_custom(t_bvh *bvh, int idx_1, int idx_2, int node_idx)
{
	t_obj_geo	*tmp;
	t_object	*object;

	tmp = bvh->obj_geo[node_idx][idx_1];
	bvh->obj_geo[node_idx][idx_1] = bvh->obj_geo[node_idx][idx_2];
	bvh->obj_geo[node_idx][idx_2] = tmp;
	object = bvh->group[node_idx][idx_1];
	bvh->group[node_idx][idx_1] = bvh->group[node_idx][idx_2];
	bvh->group[node_idx][idx_2] = object;
}
