/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:53:31 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/23 12:30:38 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	get_bonus_data_cy(t_data *data, char *specs);

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
	data->objects[data->objects_idx].mat.checker_flag = 0;
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

int	set_tri(t_obj_parser *parser, char *specs)
{
	int	len_filepath;

	len_filepath = ft_strlen(parser->filename);
	specs += len_filepath + 3;
	if (get_obj_rgb(&specs, &parser->tri_rgb[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_obj_rgb(&specs, &parser->tri_rgb[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_obj_rgb(&specs, &parser->tri_rgb[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &parser->tri_refl_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &parser->tri_refr_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_refr_idx(&specs, &parser->tri_refr_idx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}