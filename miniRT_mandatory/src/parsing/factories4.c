/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:08:59 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/25 11:36:43 by dlemaire         ###   ########.fr       */
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

