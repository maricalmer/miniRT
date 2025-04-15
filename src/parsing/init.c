/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:59:39 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/15 21:41:40 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	alloc_float_array(float (**array)[3], int count);

void	init_rt_lists(t_data *data, t_obj_parser *parsers)
{
	data->objects = malloc(sizeof(t_object) * data->n_obj);
	if (!data->objects)
	{
		if (parsers)
			free_obj_parse_2(parsers, data->n_obj_files);
		handle_memory_failure(__func__);
	}
	data->lights = malloc(sizeof(t_light) * data->n_light);
	if (!data->lights)
	{
		if (parsers)
			free_obj_parse_2(parsers, data->n_obj_files);
		free(data->objects);
		handle_memory_failure(__func__);
	}
}

int	init_obj_lists(t_obj_parser *parser)
{
	if (alloc_float_array(&parser->vertices, parser->n_v) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (alloc_float_array(&parser->normals, parser->n_vn) == EXIT_FAILURE)
		return (free(parser->vertices), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	alloc_float_array(float (**array)[3], int count)
{
	*array = malloc(sizeof(float [3]) * count);
	if (!*array)
		handle_memory_failure(__func__);
	return (EXIT_SUCCESS);
}

void	increase_if_uniq(int *value, char *specs, t_data *data)
{
	if (!*value)
		(*value)++;
	else
	{
		print_error(3);
		free(specs);
		close(data->rt_fd);
		exit(EXIT_FAILURE);
	}
}
