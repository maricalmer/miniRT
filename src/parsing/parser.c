#include <minirt.h>

int		init_elem(t_scn *scn);
void	save_elem_data(t_scn *scn);
int		create_elements(t_scn *scn);

int	handle_parsing(t_scn *scn, char *filename)
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
	if (create_elements(scn) == EXIT_FAILURE)
		return (EXIT_FAILURE);
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
		if (line[len - 1] == '\n')
			line[len - 1] = '\0';
		scn->elements[n] = line;
		n++;
	}
	scn->elements[n] = NULL;
	close(scn->fd);
}

int	create_elements(t_scn *scn)
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
		// TODO: trim white spaces
		if (scn->elements[i][0] == 'A' && !n_ambient)
		{
			create_ambient_light(scn->elements[i]);
			n_ambient++;
		}
		else if (scn->elements[i][0] == 'C' && !n_cam)
		{
			create_cam(scn->elements[i]);
			n_cam++;
		}
		else if (scn->elements[i][0] == 'L' && !n_light)
		{
			create_light(scn->elements[i]);
			n_light++;
		}
		else if (!ft_strncmp(scn->elements[i], "sp", 2))
			create_sphere(scn->elements[i]);
		else if (!ft_strncmp(scn->elements[i], "pl", 2))
			create_plane(scn->elements[i]);
		else if (!ft_strncmp(scn->elements[i], "cy", 2))
			create_cylinder(scn->elements[i]);
		else
		{
			print_error(5);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
