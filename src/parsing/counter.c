/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:25:40 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 16:35:01 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Parses an input .obj file to extract and count various 3D elements like    */
/* vertices, faces, and object types (plane, sphere...). It also checks       */
/* for proper formatting and handles errors during parsing. The counts are    */
/* stored in the provided data structure for further use.                     */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	count_obj_data(char *line, t_obj_parser *parser);

void	count_rt_scene_elems(char *parsed_line, t_data *data, int *n_cam,
			int *n_ambient)
{
	if (is_object_file(parsed_line))
		data->n_obj_files++;
	else if (is_light(parsed_line))
		data->n_light++;
	else if (is_cam(parsed_line))
		increase_if_uniq(n_cam, parsed_line, data);
	else if (is_ambient(parsed_line))
		increase_if_uniq(n_ambient, parsed_line, data);
	else if (is_plane(parsed_line) || is_sphere(parsed_line) 
		|| is_cylinder(parsed_line) || is_rectangle(parsed_line))
		data->n_obj++;
	else
	{
		free(parsed_line);
		print_error(MAND_SET_ERROR);
		close(data->rt_fd);
		exit(EXIT_FAILURE);
	}
	if (parsed_line != NULL)
		free(parsed_line);
}

void	read_obj(t_data *data, t_obj_parser *parser)
{
	int		fd;
	char	*line;

	fd = open(parser->filename, O_RDONLY);
	if (fd < 0)
		handle_file_error(__func__, parser->filename);
	line = NULL;
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (line[0] == '\n')
		{
			free(line);
			continue ;
		}
		count_obj_data(line, parser);
	}
	data->n_obj += parser->n_f;
}

static int	count_obj_data(char *line, t_obj_parser *parser)
{
	char	*parsed_line;

	parsed_line = format_string(line, ft_strlen(line));
	if (parsed_line[0] == 'v')
	{
		if (parsed_line[1] == 'n' && parsed_line[2] == ' ')
			parser->n_vn++;
		else if (parsed_line[1] == ' ')
			parser->n_v++;
		else
		{
			free(parsed_line);
			return (print_error(TRI_ERROR), EXIT_FAILURE);
		}
	}
	else if (is_face(parsed_line))
		parser->n_f++;
	else if (!is_smoothing(parsed_line))
	{
		free(parsed_line);
		return (print_error(TRI_ERROR), EXIT_FAILURE);
	}
	free(parsed_line);
	return (EXIT_SUCCESS);
}
