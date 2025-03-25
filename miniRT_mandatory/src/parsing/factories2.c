/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:49:04 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/25 11:36:23 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	create_sphere(t_data *data, char *specs)
{
	if (get_coord(&specs, data->objects[data->objects_idx].geo.sph.center)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_radius(&specs, &data->objects[data->objects_idx].geo.sph.radius)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, data->objects[data->objects_idx].mat.rgb)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->objects[data->objects_idx].type = SPHERE;
	return (EXIT_SUCCESS);
}
