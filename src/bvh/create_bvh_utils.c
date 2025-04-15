/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_bvh_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:00:36 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/15 17:18:13 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	get_group_size(t_data *data, t_bvh *bvh)
{
	t_object	*obj;
	int			i;

	obj = data->objects;
	bvh->group_size[0] = 0;
	i = -1;
	while (++i < data->n_obj)
	{
		if (obj->type == SPHERE || obj->type == TRI
			|| obj->type == RECTANGLE)
			bvh->group_size[0]++;
		obj++;
	}
}

void	create_obj_list_root(t_data *data, t_bvh *bvh)
{
	t_object	*obj;
	int			i;
	int			j;

	bvh->group[0] = malloc(sizeof(t_object *) * bvh->group_size[0]);
	if (!bvh->group[0])
		handle_memory_failure(__func__);
	obj = data->objects;
	i = -1;
	j = 0;
	while (++i < data->n_obj)
	{
		if (obj[i].type == SPHERE || obj[i].type == TRI
			|| obj[i].type == RECTANGLE)
			bvh->group[0][j++] = &obj[i];
	}
}

int	find_min_idx(int x[3])
{
	if (x[0] <= x[1] && x[0] <= x[2])
		return (0);
	if (x[1] <= x[0] && x[1] <= x[2])
		return (1);
	return (2);
}

void	malloc_groups_n_geo(t_bvh *bvh, t_cut_in_two *cut)
{
	bvh->group[cut->idx_left] = malloc(sizeof(t_object *)
			* bvh->group_size[cut->idx_left]);
	bvh->group[cut->idx_right] = malloc(sizeof(t_object *)
			* bvh->group_size[cut->idx_right]);
	bvh->obj_geo[cut->idx_left] = malloc(sizeof(t_obj_geo *)
			* bvh->group_size[cut->idx_left]);
	bvh->obj_geo[cut->idx_right] = malloc(sizeof(t_obj_geo *)
			* bvh->group_size[cut->idx_right]);
	if (!bvh->group[cut->idx_left] || !bvh->group[cut->idx_right]
		|| !bvh->obj_geo[cut->idx_left] || !bvh->obj_geo[cut->idx_right])
		handle_memory_failure(__func__);
}

void	get_mid_planes(t_bvh *bvh, int idx, t_cut_in_two *cut)
{
	int		i;

	i = -1;
	while (++i < 3)
		cut->mid[i] = find_median_custom(bvh, idx, i);
}
