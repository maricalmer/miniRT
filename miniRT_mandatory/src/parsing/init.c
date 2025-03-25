/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:59:39 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/25 16:01:15 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	init_rt_lists(t_data *data)
{
	data->objects = malloc(sizeof(t_object) * data->n_obj);
	if (!data->objects)
		return (print_error(3), EXIT_FAILURE);
	data->lights = malloc(sizeof(t_light) * data->n_light);
	if (!data->lights)
		return (free(data->objects), print_error(3), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	increase_if_uniq(int *value)
{
	if (!*value)
		(*value)++;
	else
		return (print_error(4), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
