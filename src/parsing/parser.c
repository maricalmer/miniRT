/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:38:13 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/23 23:13:30 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Reads and formats input lines, extracts essential scene elements like      */
/* cameras, lights and object file references. Ensures correct formatting     */
/* before moving to object creation.                                          */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	apply_uniform_spacing(char *specs, char *start, char *end);

void	handle_parsing(char **av, t_data *data)
{
	read_rt(data);
	if (!data->n_light)
	{
		print_error(MAND_SET_ERROR);
		exit(EXIT_FAILURE);
	}
	if (data->n_obj_files)
		parse_obj_files(data, av[1]);
	else
		init_rt_lists(data, NULL);
	create_elements_rt(data, av[1]);
}

void	read_rt(t_data *data)
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
		count_rt_elems(specs, data, &n_cam, &n_ambient);
	}
	close(data->rt_fd);
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
