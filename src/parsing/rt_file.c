#include "minirt.h"

static int	read_rt_file(t_data *data);
static int	process_rt_line(t_data *data, char *specs);
static int	handle_light_creation(t_data *data, char *specs);
static int	handle_object_creation(t_data *data, char *specs);

int	create_elements_rt(t_data *data, char *filename)
{
	data->rt_fd = open(filename, O_RDONLY);
	if (data->rt_fd < 0)
		return (print_error(3), EXIT_FAILURE);
	if (read_rt_file(data) == EXIT_FAILURE)
	{
		close(data->rt_fd);
		return (EXIT_FAILURE);
	}
	close(data->rt_fd);
	return (EXIT_SUCCESS);
}

static int	read_rt_file(t_data *data)
{
	char	*line;
	char	*specs;
	int		len;

	while (1)
	{
		line = get_next_line(data->rt_fd);
		if (line == NULL)
			break ;
		len = ft_strlen(line);
		if (line[0] == '\n' || line[0] == '+' || line[0] == '|' || !len)
		{
			free(line);
			continue ;
		}
		specs = format_string(line, len);
		if (process_rt_line(data, specs) == EXIT_FAILURE)
		{
			free(specs);
			return (EXIT_FAILURE);
		}
		free(specs);
	}
	return (EXIT_SUCCESS);
}

static int	process_rt_line(t_data *data, char *specs)
{
	if (specs[0] == 'o')
		return (EXIT_SUCCESS);
	if (specs[0] == 'p' || specs[0] == 's' || specs[0] == 'c')
	{
		if (handle_object_creation(data, specs) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else
	{
		if (handle_light_creation(data, specs) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	handle_light_creation(t_data *data, char *specs)
{
	if (specs[0] == 'L')
	{
		if (create_light(data, &specs[2]) == EXIT_FAILURE)
			return (print_error(8), EXIT_FAILURE);
	}
	else if (specs[0] == 'A')
	{
		if (create_ambient_light(data, &specs[2]) == EXIT_FAILURE)
			return (print_error(6), EXIT_FAILURE);
	}
	else if (specs[0] == 'C')
	{
		if (create_cam(data, &specs[2]) == EXIT_FAILURE)
			return (print_error(7), EXIT_FAILURE);
	}
	else
		return (print_error(5), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	handle_object_creation(t_data *data, char *specs)
{
	if (specs[0] == 'p')
	{
		if (create_plane(data, &specs[3]) == EXIT_FAILURE)
			return (print_error(10), EXIT_FAILURE);
		data->objects_idx++;
	}
	else if (specs[0] == 's')
	{
		if (create_sphere(data, &specs[3]) == EXIT_FAILURE)
			return (print_error(9), EXIT_FAILURE);
		data->objects_idx++;
	}
	else if (specs[0] == 'c')
	{
		if (create_cylinder(data, &specs[3]) == EXIT_FAILURE)
			return (print_error(11), EXIT_FAILURE);
		data->objects_idx++;
	}
	else
		return (print_error(5), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}


