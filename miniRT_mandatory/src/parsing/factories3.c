/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:53:31 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/26 11:32:53 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	create_cylinder(t_data *data, char *specs)
{
	if (get_coord(&specs, data->objects[data->objects_idx].geo.cyl.center)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_vec_norm(&specs, data->objects[data->objects_idx].geo.cyl.dir)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_radius(&specs, &data->objects[data->objects_idx].geo.cyl.radius)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_length(&specs, &data->objects[data->objects_idx].geo.cyl.height)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, data->objects[data->objects_idx].mat.rgb)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->objects[data->objects_idx].type = CYLINDER;
	return (EXIT_SUCCESS);
}
