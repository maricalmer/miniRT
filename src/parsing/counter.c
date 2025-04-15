/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:25:40 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/15 23:31:33 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	count_tri_data(char *line, t_obj_parser *parser);

void	count_rt_elems(char *specs, t_data *data, int *n_cam, int *n_ambient)
{
	if (is_object_file(specs))
		data->n_obj_files++;
	else if (is_light(specs))
		data->n_light++;
	else if (is_cam(specs))
		increase_if_uniq(n_cam, specs, data);
	else if (is_ambient(specs))
		increase_if_uniq(n_ambient, specs, data);
	else if (is_plane(specs) || is_sphere(specs) || is_cylinder(specs)
		|| is_rectangle(specs))
		data->n_obj++;
	else
	{
		free(specs);
		print_error(3);
		close(data->rt_fd);
		exit(EXIT_FAILURE);
	}
	if (specs != NULL)
		free(specs);
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
		count_tri_data(line, parser);
	}
	data->n_obj += parser->n_f;
}

static int	count_tri_data(char *line, t_obj_parser *parser)
{
	char	*specs;

	specs = format_string(line, ft_strlen(line));
	if (specs[0] == 'v')
	{
		if (specs[1] == 'n' && specs[2] == ' ')
			parser->n_vn++;
		else if (specs[1] == ' ')
			parser->n_v++;
		else
		{
			free(specs);
			return (print_error(12), EXIT_FAILURE);
		}
	}
	else if (is_face(specs))
		parser->n_f++;
	else if (!is_smoothing(specs))
	{
		free(specs);
		return (print_error(12), EXIT_FAILURE);
	}
	free(specs);
	return (EXIT_SUCCESS);
}
