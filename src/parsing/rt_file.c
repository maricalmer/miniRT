/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_file.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:39:09 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/23 17:18:34 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	read_rt_file(t_data *data);
static void	process_rt_line(t_data *data, char *specs);
static int	handle_light_creation(t_data *data, char *specs);
static int	handle_object_creation(t_data *data, char *specs);

void	create_elements_rt(t_data *data, char *filename)
{
	data->rt_fd = open(filename, O_RDONLY);
	if (data->rt_fd < 0)
		handle_file_error(__func__, filename);
	read_rt_file(data);
	if (!data->n_light)
	{
		print_error(MAND_SET_ERROR);
		exit(EXIT_FAILURE);
	}
	close(data->rt_fd);
}

static void	read_rt_file(t_data *data)
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
		if (!specs)
			continue ;
		process_rt_line(data, specs);
		free(specs);
	}
}

static void	process_rt_line(t_data *data, char *specs)
{
	if (specs[0] == 'o')
		return ;
	if (specs[0] == 'p' || specs[0] == 's' || specs[0] == 'c'
		|| specs[0] == 'r')
	{
		if (handle_object_creation(data, specs) == EXIT_FAILURE)
			abort_scene_parsing_on_failure(data, specs);
	}
	else
	{
		if (handle_light_creation(data, specs) == EXIT_FAILURE)
			abort_scene_parsing_on_failure(data, specs);
	}
}

static int	handle_light_creation(t_data *data, char *specs)
{
	if (specs[0] == 'L')
	{
		if (create_light(data, &specs[2]) == EXIT_FAILURE)
			return (print_error(LIGHT_ERROR), EXIT_FAILURE);
	}
	else if (specs[0] == 'A')
	{
		if (create_ambient_light(data, &specs[2]) == EXIT_FAILURE)
			return (print_error(AMB_ERROR), EXIT_FAILURE);
	}
	else if (specs[0] == 'C')
	{
		if (create_cam(data, &specs[2]) == EXIT_FAILURE)
			return (print_error(CAM_ERROR), EXIT_FAILURE);
	}
	else
		return (print_error(TYPES_ERROR), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	handle_object_creation(t_data *data, char *specs)
{
	if (specs[0] == 'p')
	{
		if (create_plane(data, &specs[3]) == EXIT_FAILURE)
			return (print_error(PLANE_ERROR), EXIT_FAILURE);
	}
	else if (specs[0] == 's')
	{
		if (create_sphere(data, &specs[3]) == EXIT_FAILURE)
			return (print_error(SPHERE_ERROR), EXIT_FAILURE);
	}
	else if (specs[0] == 'c')
	{
		if (create_cylinder(data, &specs[3]) == EXIT_FAILURE)
			return (print_error(CYLINDER_ERROR), EXIT_FAILURE);
	}
	else if (specs[0] == 'r')
	{
		if (create_rectangle(data, &specs[3]) == EXIT_FAILURE)
			return (print_error(RECTANGLE_ERROR), EXIT_FAILURE);
	}
	else
		return (print_error(TYPES_ERROR), EXIT_FAILURE);
	data->objects_idx++;
	return (EXIT_SUCCESS);
}
