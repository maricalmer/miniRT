#include "minirt.h"

static void	get_obj_filenames(t_obj_parser *parsers, int fd);
static int	process_obj_line(char *specs, t_data *data, t_obj_parser *parser);
static int	create_elements_obj(t_data *data, t_obj_parser *parser);
static int	handle_face_line(char *specs, t_data *data, t_obj_parser *parser);
static int	handle_vertex_line(char *specs, t_obj_parser *parser);
static int	parse_vertex(char *line, t_obj_parser *parser);
static int	parse_normal(char *line, t_obj_parser *parser);

int	parse_obj_files(t_data *data, char *filename)
{
	int				i;
	t_obj_parser	parsers[data->n_obj_files];

	init_parsers(parsers, data->n_obj_files);
	data->rt_fd = open(filename, O_RDONLY);
	if (data->rt_fd < 0)
		return (print_error(3), EXIT_FAILURE);
	get_obj_filenames(parsers, data->rt_fd);
	i = 0;
	while (i < data->n_obj_files)
	{
		if (read_and_count_data_in_obj(data, &parsers[i]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (init_elem_obj(&parsers[i]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}
	if (init_elem_rt(data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	i = 0;
	while (i < data->n_obj_files)
	{
		if (create_elements_obj(data, &parsers[i]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}
	data->tri_n = 
	i = -1;
	while (++i < data->n_obj_files)
	{
		free(parsers[i].filename);
		// free(parsers[i].vertices[0]);
		// free(parsers[i].vertices);
		// free(parsers[i].normals[0]);
		// free(parsers[i].normals);
	}
	return (EXIT_SUCCESS);
}

static void	get_obj_filenames(t_obj_parser *parsers, int fd)
{
	int				i;
	char			*line;
	char			*specs;

	line = NULL;
	i = 0;
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
		specs = format_string(line, ft_strlen(line));
		if (!is_object_file(specs))
		{
			free(specs);
			break ;
		}
		parsers[i].filename = malloc(sizeof(char) * (ft_strlen(specs) - 1));
		if (sscanf(specs, "o %s", parsers[i].filename) == 1)
			i++;
		free(specs);
	}
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
		{
			free(specs);
			return (EXIT_FAILURE);
		}
		free(specs);
	}
	return (EXIT_SUCCESS);
}

static int	process_obj_line(char *specs, t_data *data, t_obj_parser *parser)
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

static int parse_vertex(char *line, t_obj_parser *parser)
{
	if (sscanf(line, "v %f %f %f",
			&parser->vertices[parser->idx_v][0],
			&parser->vertices[parser->idx_v][1],
			&parser->vertices[parser->idx_v][2]) != 3)
		return (print_error(12), EXIT_FAILURE);
	parser->idx_v++;
	return (EXIT_SUCCESS);
}

static int parse_normal(char *line, t_obj_parser *parser)
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