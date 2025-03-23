/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:55:14 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/23 13:52:22 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	get_bonus_data_pl(t_data *data, char *specs);

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

int	create_light(t_data *data, char *specs)
{
	t_light		light;
	static int	i;

	if (get_coord(&specs, light.origin) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &light.brightness) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb_norm(&specs, light.rgb) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	// BONUS
	data->lights[i++] = light;
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
	// BONUS
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
	if (get_checkerboard_flag(&specs,
			&data->objects[data->objects_idx].mat.checker_flag) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (data->objects[data->objects_idx].mat.checker_flag)
	{
		if (get_rgb(&specs, data->objects[data->objects_idx].mat.rgb2)
			== EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (get_vec_norm(&specs, data->objects[data->objects_idx].geo.pl.u)
			== EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (get_vec_norm(&specs, data->objects[data->objects_idx].geo.pl.v)
			== EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
