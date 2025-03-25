/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:55:14 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/25 11:35:09 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	create_ambient_light(t_data *data, char *specs)
{
	t_ambient_light	ambient;

	if (get_ratio(&specs, &ambient.brightness) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb_norm(&specs, ambient.rgb) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->ambient = ambient;
	return (EXIT_SUCCESS);
}

int	create_cam(t_data *data, char *specs)
{
	t_camera	camera;

	if (get_coord(&specs, camera.origin) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_vec_norm(&specs, camera.direction) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_fov_range(&specs, &camera.fov) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->cam = camera;
	return (EXIT_SUCCESS);
}

int	create_plane(t_data *data, char *specs)
{
	if (get_coord(&specs, data->objects[data->objects_idx].geo.pl.point)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_vec_norm(&specs, 
			data->objects[data->objects_idx].geo.pl.normal) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, data->objects[data->objects_idx].mat.rgb)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->objects[data->objects_idx].type = PLANE;
	return (EXIT_SUCCESS);
}
