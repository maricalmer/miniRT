/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_group.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:05:07 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/22 23:04:57 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* File replaces the current object list with a new array starting with BVH   */
/* as single object + all non-geo objects. Old list is preserved in           */
/* data->all_objects.                                                         */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	update_group(t_data *data, t_bvh *bvh)
{
	t_object	*new_objects;
	int			n_pl;
	int			i;
	int			j;

	i = -1;
	n_pl = 0;
	while (++i < data->n_obj)
		if (data->objects[i].type != SPHERE && data->objects[i].type != TRI
			&& data->objects[i].type != RECTANGLE)
			n_pl++;
	new_objects = aligned_alloc(ALGN_OBJ_ARR, sizeof(t_object) * (n_pl + 1));
	if (!new_objects)
		handle_memory_failure(__func__);
	new_objects[0].type = BVH;
	new_objects[0].geo.bvh = (void *)bvh;
	i = -1;
	j = 1;
	while (++i < data->n_obj)
		if (data->objects[i].type != SPHERE && data->objects[i].type != TRI
			&& data->objects[i].type != RECTANGLE)
			ft_memcpy(&new_objects[j++], &data->objects[i], sizeof(t_object));
	data->all_objects = data->objects;
	data->objects = new_objects;
	data->n_obj = n_pl + 1;
}
