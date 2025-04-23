/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:59:58 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/23 23:03:10 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Extracts object file paths from the scene file, loads and reads each .obj  */
/* file, initializes data structures for geometry and materials and           */
/* integrates the parsed elements into the main scene data.                   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	get_obj_filenames(t_obj_parser *parsers, t_data *data,
				char *filename);
static void	process_obj_file(t_obj_parser *parsers, char *specs);
static int	create_elements_obj(t_data *data, t_obj_parser *parser);
int			read_obj_file(t_data *data, t_obj_parser *parser);

void	parse_obj_files(t_data *data, char *filename)
{
	int				i;
	t_obj_parser	*parsers;

	parsers = ft_calloc(data->n_obj_files, sizeof(t_obj_parser));
	if (!parsers)
		handle_memory_failure(__func__);
	get_obj_filenames(parsers, data, filename);
	i = -1;
	while (++i < data->n_obj_files)
	{
		read_obj(data, &parsers[i]);
		if (init_obj_lists(&parsers[i]) == EXIT_FAILURE)
			exit_with_obj_parser_cleanup(parsers, data->n_obj_files);
	}
	init_rt_lists(data, parsers);
	i = -1;
	while (++i < data->n_obj_files)
	{
		if (create_elements_obj(data, &parsers[i]) == EXIT_FAILURE)
			cleanup_obj_parser_and_exit(parsers, data->n_obj_files);
	}
	print_tri_count(parsers[0].n_f);
	free_obj_parser_resources(parsers, data->n_obj_files);
}

static void	get_obj_filenames(t_obj_parser *parsers, t_data *data,
	char *filename)
{
	char	*line;
	char	*specs;

	data->rt_fd = open(filename, O_RDONLY);
	if (data->rt_fd < 0)
	{
		free(parsers);
		handle_file_error(__func__, filename);
	}
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
}

static void	process_obj_file(t_obj_parser *parsers, char *specs)
{
	static int	i;
	char		*name_start;
	char		*name_end;
	size_t		len;

	name_start = specs + FILENAME_OFFSET;
	while (*name_start == ' ')
		name_start++;
	name_end = name_start;
	while (*name_end && !ft_iswhitespace(*name_end))
		name_end++;
	len = name_end - name_start;
	parsers[i].filename = malloc(len + 1);   
	if (!parsers[i].filename)
		handle_memory_failure(__func__);
	ft_strlcpy(parsers[i].filename, name_start, len + 1);
	if (set_tri(&parsers[i], specs) == EXIT_FAILURE)
	{
		free(specs);
		print_error(OBJ_ERROR);
		exit_with_obj_parser_cleanup(parsers, i);
	}
}

static int	create_elements_obj(t_data *data, t_obj_parser *parser)
{
	data->obj_fd = open(parser->filename, O_RDONLY);
	if (data->obj_fd < 0)
		handle_file_error(__func__, parser->filename);
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
