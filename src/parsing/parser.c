#include <minirt.h>

int		init_elem(t_scn *scn);
void	save_elem_data(t_scn *scn);
int		create_elements(t_data *data, t_scn *scn);
char	*format_string(char *str, int len);
void	apply_uniform_spacing(char *specs, char *start, char *end);
void	init_object_and_lights(t_data *data, t_scn *scn);

int	handle_parsing(t_data *data, t_scn *scn, char *filename)
{
	if (init_elem(scn) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	close(scn->fd);
	scn->fd = open(filename, O_RDONLY);
	if (scn->fd < 0)
	{
		print_error(3);
		return (EXIT_FAILURE);
	}
	save_elem_data(scn);
	init_object_and_lights(data, scn);
	if (create_elements(data, scn) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	// check presence of cam, light, ambient
	return (EXIT_SUCCESS);
}

int	init_elem(t_scn *scn)
{
	char	*line;
	int		n;

	line = NULL;
	n = 0;
	while (1)
	{
		line = get_next_line(scn->fd);
		if (line == NULL)
			break ;
		n++;
		if (line != NULL)
			free(line);
	}
	scn->elements = malloc(sizeof(char *) * (n + 1));
	if (!scn->elements)
	{
		close(scn->fd);
		print_error(4);
		return (EXIT_FAILURE);
	}
	close(scn->fd);
	return (EXIT_SUCCESS);
}

void	save_elem_data(t_scn *scn)
{
	int		n;
	int		len;
	char	*line;

	n = 0;
	line = NULL;
	while (1)
	{
		line = get_next_line(scn->fd);
		if (line == NULL)
			break ;
		len = ft_strlen(line);
		if (!ft_strncmp(line, "\n", 1) || !len)
		{
			free(line);
			continue ;
		}
		scn->elements[n] = format_string(line, len);
		n++;
	}
	scn->elements[n] = NULL;
	close(scn->fd);
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

void	init_object_and_lights(t_data *data, t_scn *scn)
{
	int	i;
	
	while (*scn->elements)
	{
		if (!ft_strncmp(*scn->elements, "sp", 2)
			|| !ft_strncmp(*scn->elements, "pl", 2)
			|| !ft_strncmp(*scn->elements, "cy", 2))
			data->n_obj++;
		if (*scn->elements[0] == 'L')
			data->n_light++;
		scn->elements++;
	}
	data->objects = malloc(sizeof(t_object) * data->n_obj);
	i = 0;
	while (i < data->n_obj)
	{
		data->objects[i].type = EMPTY;
		i++;
	}
	data->lights = malloc(sizeof(t_light) * data->n_light);
	i = 0;
	while (i < data->n_light)
	{
		data->lights[i].rgb[0] = -1;
		i++;
	}
}

int	create_elements(t_data *data, t_scn *scn)
{
	int	i;
	int	n_ambient;
	int	n_cam;
	int	n_light;

	i = 0;
	n_ambient = 0;
	n_cam = 0;
	n_light = 0;
	while (scn->elements[i])
	{
		if (!n_ambient && scn->elements[i][0] == 'A')
		{
			if (create_ambient_light(data, &scn->elements[i][2]) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			n_ambient++;
		}
		else if (!n_cam && scn->elements[i][0] == 'C')
		{
			if (create_cam(data, &scn->elements[i][2]) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			n_cam++;
		}
		else if (!n_light && scn->elements[i][0] == 'L')
		{
			if (create_light(data, &scn->elements[i][2]) == EXIT_FAILURE);
				return (EXIT_FAILURE);
			n_light++;
		}
		else if (!ft_strncmp(scn->elements[i], "sp", 2))
			create_sphere(data, &scn->elements[i][3]);
		else if (!ft_strncmp(scn->elements[i], "pl", 2))
			create_plane(data, &scn->elements[i][3]);
		else if (!ft_strncmp(scn->elements[i], "cy", 2))
			create_cylinder(data, &scn->elements[i][3]);
		else
		{
			print_error(5);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
