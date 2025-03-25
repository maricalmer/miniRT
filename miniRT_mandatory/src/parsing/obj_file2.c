/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_file2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 11:25:31 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/23 13:54:29 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	handle_vertex_line(char *specs, t_obj_parser *parser);
static int	parse_vertex(char *line, t_obj_parser *parser);
static int	parse_normal(char *line, t_obj_parser *parser);
static int	handle_face_line(char *specs, t_data *data, t_obj_parser *parser);

int	process_obj_line(char *specs, t_data *data, t_obj_parser *parser)
{
	if (specs[0] == 'v')
		return (handle_vertex_line(specs, parser));
	else if (specs[0] == 'f')
		return (handle_face_line(specs, data, parser));
	else if (specs[0] == 's')
		return (EXIT_SUCCESS);
	else
		return (print_error(5), EXIT_FAILURE);
}

static int	handle_vertex_line(char *specs, t_obj_parser *parser)
{
	if (specs[1] == 'n')
	{
		if (parse_normal(specs, parser) == EXIT_FAILURE)
			return (print_error(12), EXIT_FAILURE);
	}
	else
	{
		if (parse_vertex(specs, parser) == EXIT_FAILURE)
			return (print_error(12), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	parse_vertex(char *line, t_obj_parser *parser)
{
	if (sscanf(line, "v %f %f %f",
			&parser->vertices[parser->idx_v][0],
		&parser->vertices[parser->idx_v][1],
			&parser->vertices[parser->idx_v][2]) != 3)
		return (print_error(12), EXIT_FAILURE);
	parser->idx_v++;
	return (EXIT_SUCCESS);
}

static int	parse_normal(char *line, t_obj_parser *parser)
{
	if (sscanf(line, "vn %f %f %f",
			&parser->normals[parser->idx_n][0],
		&parser->normals[parser->idx_n][1],
			&parser->normals[parser->idx_n][2]) != 3)
		return (print_error(12), EXIT_FAILURE);
	parser->idx_n++;
	return (EXIT_SUCCESS);
}

static int	handle_face_line(char *specs, t_data *data, t_obj_parser *parser)
{
	if (create_triangle(data, specs, parser) == EXIT_FAILURE)
		return (print_error(12), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
