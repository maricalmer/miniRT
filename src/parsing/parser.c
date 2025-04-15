/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:38:13 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/15 17:24:38 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	apply_uniform_spacing(char *specs, char *start, char *end);

int	handle_parsing(char **av, t_data *data)
{
	if (read_rt(data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (!data->n_light)
		return (print_error(3), EXIT_FAILURE);
	if (data->n_obj_files)
	{
		if (parse_obj_files(data, av[1]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else
	{
		if (init_rt_lists(data) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (create_elements_rt(data, av[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	read_rt(t_data *data)
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
		if (!specs)
			continue ;
		if (count_rt_elems(specs, data, &n_cam, &n_ambient) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (close(data->rt_fd), EXIT_SUCCESS);
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
	if (!str[i])
		return (free(str), NULL);
	start = &str[i];
	while (len > 0 && ft_iswhitespace(str[len - 1]))
		len--;
	end = &str[len - 1];
	size = end - start + 1;
	specs = malloc(sizeof(char) * (size + 1));
	if (!specs)
	{
		free(str);
		handle_memory_failure(__func__);
	}
	apply_uniform_spacing(specs, start, end);
	free(str);
	return (specs);
}

static void	apply_uniform_spacing(char *specs, char *start, char *end)
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
