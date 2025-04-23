/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:08:59 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/23 22:53:15 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Creation of light and triangle objects. It parses core parameters like     */
/* light position, brightness and color data. Processes material properties   */
/* for triangles loaded from OBJ files, including colors, reflection and      */
/* refraction attributes.                                                     */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
	data->lights[i++] = light;
	return (EXIT_SUCCESS);
}

int	set_tri(t_obj_parser *parser, char *specs)
{
	int	len_filepath;

	len_filepath = ft_strlen(parser->filename);
	specs += len_filepath + 3;
	if (get_obj_rgb(&specs, &parser->tri_rgb[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (*specs != ',')
		return (EXIT_FAILURE);
	specs++;
	if (get_obj_rgb(&specs, &parser->tri_rgb[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (*specs != ',')
		return (EXIT_FAILURE);
	specs++;
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
