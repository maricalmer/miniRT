/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:55:14 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/23 22:40:23 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Creation of ambient lights, cameras, and planes. It parses core parameters */
/* like position, color, direction, and field of view, as well as optional    */
/* extended material data like reflectivity, refractivity, and checkerboard   */
/* settings.                                                                  */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	get_bonus_data_pl(t_data *data, char *specs);
static int	get_checker_data_pl(t_data *data, char *specs);

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
	if (get_coord(&specs, data->cam.origin) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_vec_norm(&specs, data->cam.direction) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_fov_range(&specs, &data->cam.fov) == EXIT_FAILURE)
		return (EXIT_FAILURE);
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
	if (get_bonus_data_pl(data, specs) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	get_bonus_data_pl(t_data *data, char *specs)
{
	if (get_ratio(&specs, &data->objects[data->objects_idx].mat.refl_coeff)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &data->objects[data->objects_idx].mat.refr_coeff)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_refr_idx(&specs, &data->objects[data->objects_idx].mat.refr_idx)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_pos_float(&specs,
			&data->objects[data->objects_idx].mat.checker_size) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (data->objects[data->objects_idx].mat.checker_size)
	{
		if (get_checker_data_pl(data, specs) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	get_checker_data_pl(t_data *data, char *specs)
{
	data->objects[data->objects_idx].mat.checker_size = 1 
		/ data->objects[data->objects_idx].mat.checker_size;
	if (get_rgb(&specs, data->objects[data->objects_idx].mat.rgb2)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_vec_norm(&specs, data->objects[data->objects_idx].geo.pl.u)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_vec_norm(&specs, data->objects[data->objects_idx].geo.pl.v)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
