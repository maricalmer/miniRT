#include "minirt.h"

int		init_elem_rt(t_data *data);
int		init_elem_obj(t_obj_parser *parser);
int		save_elem_data_from_rt(int *fd, char **elements, int *i, char *filename);
int		save_elem_data_from_obj(t_scn *scn);
int		create_elements_rt(t_data *data, t_scn *scn, char *filename, int *idx);
int		create_elements_obj(t_data *data, t_scn *scn, char *filename, int *idx);
char	*format_string(char *str, int len);
void	apply_uniform_spacing(char *specs, char *start, char *end);
void	init_object_and_lights(t_data *data, t_scn *scn);
void	count_lines_from_rt(int fd, t_data *data);
void	count_lines_from_obj(int fd, t_obj_parser *obj_parser);

int	handle_parsing(t_data *data, t_scn *scn, int ac, char **av)
{
	t_obj_parser	obj_parser;
	int				idx;

	idx = 0;
	if (ac == 2)
	{
		count_lines_from_rt(scn->rt_fd, data);
		
		
		if (init_elem_rt(data) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (create_elements_rt(data, scn, av[1], &idx) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		
	}
	if (ac == 3)
	{
		count_lines_from_rt(scn->rt_fd, data);
		obj_parser.n_f = 0;
		obj_parser.n_v = 0;
		obj_parser.n_vn = 0;
		obj_parser.idx_n = 0;
		obj_parser.idx_v = 0;
		scn->obj_parser = &obj_parser;
		count_lines_from_obj(scn->obj_fd, &obj_parser);
		data->n_obj += obj_parser.n_f;
		if (init_elem_rt(data) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (init_elem_obj(&obj_parser) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (create_elements_rt(data, scn, av[1], &idx) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (create_elements_obj(data, scn, av[2], &idx) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	count_lines_from_rt(int fd, t_data *data)
{
	char	*line;

	line = NULL;
	data->n_obj = 0;
	data->n_light = 0;
	data->n_obj_files = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (line[0] == 'p' || line[0] == 's' || line[0] == 'c')
			data->n_obj++;
		else if (line[0] == 'L')
			data->n_light++;
		else if (line[0] == 'o')
			data->n_obj_files++;
		if (line != NULL)
			free(line);
	}
	close(fd);
}

void	count_lines_from_obj(int fd, t_obj_parser *obj_parser)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (line[0] == 'f')
			obj_parser->n_f++;
		else if (line[0] == 'v')
		{
			if (line[1] == 'n')
				obj_parser->n_vn++;
			else
				obj_parser->n_v++;
		}
		if (line != NULL)
			free(line);
	}
	close(fd);
}

int	init_elem_rt(t_data *data)
{
	int	i;

	data->objects = malloc(sizeof(t_object) * data->n_obj);
	if (!data->objects)
	{
		print_error(4);
		return (EXIT_FAILURE);
	}
	data->lights = malloc(sizeof(t_light) * data->n_light);
	if (!data->lights)
	{
		print_error(4);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (i < data->n_light)
	{
		data->lights[i].rgb[0] = -1;
		i++;
	}
	if (data->n_obj_files)
		data->obj_files = malloc(sizeof(char *) * data->n_obj_files);

	/// NEEDS ALLOCATION FOR THE STRINGS
	return (EXIT_SUCCESS);
}

int	init_elem_obj(t_obj_parser *parser)
{
	int		i;
	float	*vertex_block;
	float	*normal_block;
	int		*face_block;

	parser->vertices = malloc(sizeof(float *) * parser->n_v);
	if (!parser->vertices)
		return (print_error(4), EXIT_FAILURE);
	vertex_block = malloc(sizeof(float) * 3 * parser->n_v);
	if (!vertex_block)
	{
		free(parser->vertices);
		return (print_error(4), EXIT_FAILURE);
	}
	i = 0;
	while (i < parser->n_v)
	{
		parser->vertices[i] = vertex_block + (i * 3);
		i++;
	}
	parser->normals = malloc(sizeof(float *) * parser->n_vn);
	if (!parser->normals)
		return (print_error(4), EXIT_FAILURE);
	normal_block = malloc(sizeof(float) * 3 * parser->n_vn);
	if (!normal_block)
	{
		free(parser->vertices);
		free(vertex_block);
		return (print_error(4), EXIT_FAILURE);
	}
	i = 0;
	while (i < parser->n_vn)
	{
		parser->normals[i] = normal_block + (i * 3);
		i++;
	}
	parser->faces = malloc(sizeof(int *) * parser->n_f);
	if (!parser->faces)
		return (print_error(4), EXIT_FAILURE);
	face_block = malloc(sizeof(int) * 6 * parser->n_f);
	if (!face_block)
	{
		free(parser->vertices);
		free(vertex_block);
		free(parser->normals);
		free(normal_block);
		return (print_error(4), EXIT_FAILURE);
	}
	i = 0;
	while (i < parser->n_f)
	{
		parser->faces[i] = face_block + (i * 6);
		i++;
	}
	return (EXIT_SUCCESS);
}

void parse_vertex(char *line, t_obj_parser *parser, int *idx_v)
{
	if (*idx_v >= parser->n_v)
    {
        fprintf(stderr, "Error: Too many vertex definitions\n");
        return;
    }
	if (sscanf(line, "v %f %f %f",
           &parser->vertices[*idx_v][0],
           &parser->vertices[*idx_v][1],
           &parser->vertices[*idx_v][2]) != 3)
	{
		fprintf(stderr, "Error: Invalid vertex format: %s\n", line);
		return ;
	}
	(*idx_v)++;
}

void parse_normal(char *line, t_obj_parser *parser, int *idx_n)
{
	if (*idx_n >= parser->n_vn)
    {
        fprintf(stderr, "Error: Too many normal definitions\n");
        return;
    }
	if (sscanf(line, "vn %f %f %f",
           &parser->normals[*idx_n][0],
           &parser->normals[*idx_n][1],
           &parser->normals[*idx_n][2]) != 3)
	{
		fprintf(stderr, "Error: Invalid normal format: %s\n", line);
		return ;
	}
	(*idx_n)++;
}

char	*format_string(char *str, int len)
{
	char	*specs;
	int		i;
	int		size;
	char	*start;
	char	*end;

	i = 0;
	while (ft_iswhitespace(str[i]))
		i++;
	start = &str[i];
	while (len > 0 && ft_iswhitespace(str[len - 1]))
		len--;
	end = &str[len - 1];
	size = end - start + 1;
	specs = malloc(sizeof(char) * (size + 1));
	if (!specs)
		return (NULL);
	apply_uniform_spacing(specs, start, end);
	free(str);
	return (specs);
}

void	apply_uniform_spacing(char *specs, char *start, char *end)
{
	int	i;
	int	j;
	int	space_flag;

	i = 0;
	j = 0;
	space_flag = 0;
	while (start[i] && &start[i] <= end)
	{
		if (ft_iswhitespace(start[i]))
		{
			if (!space_flag)
			{
				specs[j++] = ' ';
				space_flag = 1;
			}
		}
		else
		{
			specs[j++] = start[i];
			space_flag = 0;
		}
		i++;
	}
	specs[j] = '\0';
}

int	create_elements_obj(t_data *data, t_scn *scn, char *filename, int *idx)
{
	char	*line;
	char	*specs;
	int		len;

	line = NULL;
	scn->obj_fd = open(filename, O_RDONLY);
	while (1)
	{
		line = get_next_line(scn->obj_fd);
		if (line == NULL)
			break ;
		len = ft_strlen(line);
		if (line[0] == '\n' || !len)
		{
			free(line);
			continue ;
		}
		specs = format_string(line, len);
		if (specs[0] == 'v')
		{
			if (specs[1] == 'n')
				parse_normal(specs, scn->obj_parser, &(scn->obj_parser->idx_n));
			else
				parse_vertex(specs, scn->obj_parser, &(scn->obj_parser->idx_v));
		}
		else if (specs[0] == 'f')
			if (create_triangle(data, specs, scn->obj_parser, idx) == EXIT_FAILURE)
				return (print_error(10), EXIT_FAILURE);
		else if (specs[0] == 'o')
			if (save_obj_filename(data, specs, scn->obj_parser, idx) == EXIT_FAILURE)
				return (print_error(10), EXIT_FAILURE);
		else
			return (print_error(5), EXIT_FAILURE);
		if (specs != NULL)
			free(specs);
	}
	close(scn->obj_fd);
	return (EXIT_SUCCESS);
}


int	create_elements_rt(t_data *data, t_scn *scn, char *filename, int *idx)
{
	char	*line;
	char	*specs;
	int		len;
	int		n_ambient;
	int		n_cam;

	line = NULL;
	n_ambient = 0;
	n_cam = 0;

	scn->rt_fd = open(filename, O_RDONLY);
	while (1)
	{
		line = get_next_line(scn->rt_fd);
		if (line == NULL)
			break ;
		len = ft_strlen(line);
		if (line[0] == '\n' || !len)
		{
			free(line);
			continue ;
		}
		specs = format_string(line, len);
		if (specs[0] == 'p')
		{
			if (create_plane(data, &specs[3], *idx/*- n_light*/ ) == EXIT_FAILURE)
				return (print_error(10), EXIT_FAILURE);
			(*idx)++;
		}
		else if (specs[0] == 's')
		{
			if (create_sphere(data, &specs[3], *idx/*- n_light*/ ) == EXIT_FAILURE)
				return (print_error(9), EXIT_FAILURE);
			(*idx)++;
		}
		else if (specs[0] == 'c')
		{
			if (create_cylinder(data, &specs[3], *idx/*- n_light*/ ) == EXIT_FAILURE)
				return (print_error(11), EXIT_FAILURE);
			(*idx)++;
		}
		else if (specs[0] == 'L')
		{
			if (create_light(data, &specs[2]) == EXIT_FAILURE)
				return (print_error(8), EXIT_FAILURE);
		}
		else if (!n_ambient && specs[0] == 'A')
		{
			if (create_ambient_light(data, &specs[2]) == EXIT_FAILURE)
				return (print_error(6), EXIT_FAILURE);
			n_ambient++;
		}
		else if (!n_cam && specs[0] == 'C')
		{
			if (create_cam(data, &specs[2]) == EXIT_FAILURE)
				return (print_error(7), EXIT_FAILURE);
			n_cam++;
		}
		else
			return (print_error(5), EXIT_FAILURE);
		if (specs != NULL)
			free(specs);
	}
	close(scn->rt_fd);
	return (EXIT_SUCCESS);
}
