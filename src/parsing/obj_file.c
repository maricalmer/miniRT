/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:59:58 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/23 12:43:31 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	get_obj_filenames(t_obj_parser *parsers, t_data *data,
				char *filename);
static int	process_obj_file(t_obj_parser *parsers, char *specs);
static int	create_elements_obj(t_data *data, t_obj_parser *parser);
int			read_obj_file(t_data *data, t_obj_parser *parser);

int	parse_obj_files(t_data *data, char *filename)
{
	int				i;
	t_obj_parser	*parsers;

	parsers = ft_calloc(data->n_obj_files, sizeof(t_obj_parser));
	if (!parsers)
		return (print_error(3), EXIT_FAILURE);
	if (get_obj_filenames(parsers, data, filename) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	i = -1;
	while (++i < data->n_obj_files)
	{
		if (read_obj(data, &parsers[i]) == EXIT_FAILURE 
			|| init_obj_lists(&parsers[i]) == EXIT_FAILURE)
			return (free_obj_parse_1(parsers, data->n_obj_files), EXIT_FAILURE);
	}
	if (init_rt_lists(data) == EXIT_FAILURE)
		return (free_obj_parse_2(parsers, data->n_obj_files), EXIT_FAILURE);
	i = -1;
	while (++i < data->n_obj_files)
	{
		if (create_elements_obj(data, &parsers[i]) == EXIT_FAILURE)
			return (free_obj_parse_2(parsers, data->n_obj_files), EXIT_FAILURE);
	}
	print_tri_count(parsers[0].n_f);
	return (free_obj_parse_2(parsers, data->n_obj_files), EXIT_SUCCESS);
}

static int	get_obj_filenames(t_obj_parser *parsers, t_data *data,
	char *filename)
{
	char	*line;
	char	*specs;

	data->rt_fd = open(filename, O_RDONLY);
	if (data->rt_fd < 0)
		return (free(parsers), print_error(3), EXIT_FAILURE);
	line = NULL;
	while (1)
	{
		line = get_next_line(data->rt_fd);
		if (line == NULL)
			break ;
		if (line[0] == '\n' || line[0] == '+' || line[0] == '|')
		{
			free(line);
			continue ;
		}
		specs = format_string(line, ft_strlen(line));
		if (is_object_file(specs))
			process_obj_file(parsers, specs);
		free(specs);
	}
	return (EXIT_SUCCESS);
}

static int	process_obj_file(t_obj_parser *parsers, char *specs)
{
	static int	i;

	parsers[i].filename = malloc(sizeof(char) * (ft_strlen(specs) - 1));
	if (!parsers[i].filename)
		return (print_error(3), EXIT_FAILURE);
	if (sscanf(specs, "o %s", parsers[i].filename) == 1)
		set_tri(&parsers[i], specs);
	i++;
	return (EXIT_SUCCESS);
}

static int	create_elements_obj(t_data *data, t_obj_parser *parser)
{
	data->obj_fd = open(parser->filename, O_RDONLY);
	if (data->obj_fd < 0)
		return (print_error(3), EXIT_FAILURE);
	if (read_obj_file(data, parser) == EXIT_FAILURE)
	{
		close(data->obj_fd);
		return (EXIT_FAILURE);
	}
	close(data->obj_fd);
	return (EXIT_SUCCESS);
}

int	read_obj_file(t_data *data, t_obj_parser *parser)
{
	char	*line;
	char	*specs;
	int		len;

	while (1)
	{
		line = get_next_line(data->obj_fd);
		if (line == NULL)
			break ;
		len = ft_strlen(line);
		if (line[0] == '\n' || !len)
		{
			free(line);
			continue ;
		}
		specs = format_string(line, len);
		if (process_obj_line(specs, data, parser) == EXIT_FAILURE)
			return (free(specs), EXIT_FAILURE);
		free(specs);
	}
	return (EXIT_SUCCESS);
}
