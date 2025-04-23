/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:37:58 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/23 11:57:02 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* File replaces the current object list with a new array starting with BVH   */
/* as single object + all non-geo objects. Old list is preserved in           */
/* data->all_objects.                                                         */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* File provides utility functions to handle cleanup during scene parsing     */
/* failure.                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_obj_parser_resources(t_obj_parser *parsers, int n_files)
{
	int	i;

	i = -1;
	while (++i < n_files)
	{
		free(parsers[i].filename);
		free(parsers[i].vertices);
		free(parsers[i].normals);
	}
	free(parsers);
}

void	cleanup_obj_parser_and_exit(t_obj_parser *parsers, int n_files)
{
	free_obj_parser_resources(parsers, n_files);
	exit(EXIT_FAILURE);
}

void	abort_scene_parsing_on_failure(t_data *data, char *specs)
{
	free(specs);
	free(data->objects);
	free(data->lights);
	close(data->rt_fd);
	exit(EXIT_FAILURE);
}
