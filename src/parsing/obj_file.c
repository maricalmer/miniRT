#include "minirt.h"

static int	get_obj_filenames(t_obj_parser *parsers, t_data *data, char *filename);
static int	process_obj_line(char *specs, t_data *data, t_obj_parser *parser);
static int	create_elements_obj(t_data *data, t_obj_parser *parser);
static int	handle_face_line(char *specs, t_data *data, t_obj_parser *parser);
static int	handle_vertex_line(char *specs, t_obj_parser *parser);
static int	parse_vertex(char *line, t_obj_parser *parser);
static int	parse_normal(char *line, t_obj_parser *parser);
static int	set_tri(t_obj_parser *parser, char *specs);

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
		if (read_obj(data, &parsers[i]) == EXIT_FAILURE)
			return (free_obj_parse_1(parsers, data->n_obj_files), EXIT_FAILURE);
		if (init_obj_lists(&parsers[i]) == EXIT_FAILURE)
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
	printf("  [TRIMESH]\n\n");
	printf("    > %d tris\n\n", parsers[0].n_f);
	return (free_obj_parse_2(parsers, data->n_obj_files), EXIT_SUCCESS);
}

static int	get_obj_filenames(t_obj_parser *parsers, t_data *data, char *filename)
{
	int				i;
	char			*line;
	char			*specs;

	line = NULL;
	i = 0;
	data->rt_fd = open(filename, O_RDONLY);
	if (data->rt_fd < 0)
		return (free(parsers), print_error(3), EXIT_FAILURE);
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
		{
			parsers[i].filename = malloc(sizeof(char) * (ft_strlen(specs) - 1));
			if (sscanf(specs, "o %s", parsers[i].filename) == 1)
			{
				set_tri(&parsers[i], specs);
				i++;
			}
		}
		free(specs);
	}
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

static int	set_tri(t_obj_parser *parser, char *specs)
{
	int len_filepath;

	len_filepath = ft_strlen(parser->filename);
	specs += len_filepath + 3;
	if (get_obj_rgb(&specs, &parser->tri_rgb[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_obj_rgb(&specs, &parser->tri_rgb[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_obj_rgb(&specs, &parser->tri_rgb[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &parser->tri_refl_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &parser->tri_refr_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_refr_idx(&specs, &parser->tri_refr_idx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
