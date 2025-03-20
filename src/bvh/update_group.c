/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_group.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:05:07 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/03/19 21:05:12 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	update_group(t_data *data, t_bvh *bvh)
{
	t_object	*new_objects;
	int			plane_counter;
	int			i;
	int			j;

	i = -1;
	plane_counter = 0;
	while (++i < data->n_obj)
		if (data->objects[i].type != SPHERE && data->objects[i].type != TRI)
			plane_counter++;
	new_objects = aligned_alloc(64, sizeof(t_object) * (plane_counter + 1));
	new_objects[0].type = BVH;
	new_objects[0].geo.bvh = (void *)bvh;
	i = -1;
	j = 1;
	while (++i < data->n_obj)
		if (data->objects[i].type != SPHERE && data->objects[i].type != TRI)
			ft_memcpy(&new_objects[j++], &data->objects[i], sizeof(t_object));
	data->all_objects = data->objects;
	data->objects = new_objects;
	data->n_obj = plane_counter + 1;
}
