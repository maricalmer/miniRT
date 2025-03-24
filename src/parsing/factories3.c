/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:53:31 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/24 16:14:11 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	get_bonus_data_cy(t_data *data, char *specs);
static int	get_bonus_data_rec(t_data *data, char *specs);
static int	get_checker_size_data_rec(t_data *data, char *specs);

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
	data->objects[data->objects_idx].mat.checker_size = 0;
	// BONUS
	if (get_bonus_data_cy(data, specs) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	get_bonus_data_cy(t_data *data, char *specs)
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
	return (EXIT_SUCCESS);
}

int	create_rectangle(t_data *data, char *specs)
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
	data->objects[data->objects_idx].type = RECTANGLE;
	// BONUS
	if (get_bonus_data_rec(data, specs) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	get_bonus_data_rec(t_data *data, char *specs)
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
			&data->objects[data->objects_idx].geo.rec.uv_size) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_pos_float(&specs,
			&data->objects[data->objects_idx].mat.checker_size) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (data->objects[data->objects_idx].mat.checker_size)
		if (get_checker_size_data_rec(data, specs) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	get_checker_size_data_rec(t_data *data, char *specs)
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
