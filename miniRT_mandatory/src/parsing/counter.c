/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:25:40 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/25 15:23:07 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	count_rt_elems(char *specs, t_data *data, int *n_cam, int *n_ambient)
{
	if (is_light(specs))
		data->n_light++;
	else if (is_cam(specs))
	{
		if (increase_if_uniq(n_cam) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (is_ambient(specs))
	{
		if (increase_if_uniq(n_ambient) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (is_plane(specs) || is_sphere(specs) || is_cylinder(specs))
		data->n_obj++;
	else
	{
		free(specs);
		return (print_error(5), EXIT_FAILURE);
	}
	if (specs != NULL)
		free(specs);
	return (EXIT_SUCCESS);
}

