/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:08:59 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/24 16:22:44 by dlemaire         ###   ########.fr       */
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
	// BONUS
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
