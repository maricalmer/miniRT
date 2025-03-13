#include "minirt.h"

static int	count_objects(char *specs, t_data *data, int *n_cam, int *n_ambient);
static int	count_tri_data(char *line, t_obj_parser *parser);

int	read_and_count_data_in_rt(t_data *data)
{
	char	*line;
	int		n_cam;
	int		n_ambient;
	char	*specs;

	line = NULL;
	n_cam = 0;
	n_ambient = 0;
	while (1)
	{
		line = get_next_line(data->rt_fd);
		if (line == NULL)
			break ;
		if (line[0] == '\n' || line[0] == '+' || line[0] == '|' )
		{
			free(line);
			continue ;
		}
		specs = format_string(line, ft_strlen(line));
		if (count_objects(specs, data, &n_cam, &n_ambient) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	close(data->rt_fd);
	if (!data->n_light)
		return (print_error(4), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	count_objects(char *specs, t_data *data, int *n_cam, int *n_ambient)
{
	if (is_object_file(specs))
		data->n_obj_files++;
	else if (is_light(specs))
		data->n_light++;
	else if (is_cam(specs))
	{
		if (increase_if_uniq(n_cam) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (is_ambient(specs))
	{
		if (increase_if_uniq(n_ambient) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (is_plane(specs) || is_sphere(specs) || is_cylinder(specs))
		data->n_obj++;
	else
	{
		free(specs);
		return (print_error(5), EXIT_FAILURE);
	}
	if (specs != NULL)
		free(specs);
	return (EXIT_SUCCESS);
}

int	read_and_count_data_in_obj(t_data *data, t_obj_parser *parser)
{
	int		fd;
	char	*line;

	fd = open(parser->filename, O_RDONLY);
	if (fd < 0)
		return (print_error(3), EXIT_FAILURE);
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
	return (EXIT_SUCCESS);
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

