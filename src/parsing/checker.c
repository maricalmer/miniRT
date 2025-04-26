/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:25:18 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 16:29:18 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Validates the command-line arguments for the correct input file format.    */
/* Ensures that exactly one argument is provided, and the file has a ".rt"    */
/* extension. The file is then opened for reading. If any error occurs, an    */
/* appropriate error message is printed, and the program exits.               */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	has_rt_extension(char *str);
static void	check_filename(char *filename, int *fd);

void	check_input(int ac, char **av, t_data *data)
{
	if (ac != 2)
	{
		print_error(ARGS_ERROR);
		exit(EXIT_FAILURE);
	}
	check_filename(av[1], &data->rt_fd); 
}

static void	check_filename(char *filename, int *fd)
{
	if (!ft_strncmp(filename, "", 1) || !has_rt_extension(filename))
	{
		print_error(FILE_ERROR);
		exit(EXIT_FAILURE);
	}
	*fd = open(filename, O_RDONLY);
	if (*fd < 0)
		handle_file_error(__func__, filename);
}

static int	has_rt_extension(char *str)
{
	int	str_len;

	str_len = ft_strlen(str);
	if (str_len < 4)
		return (0);
	if (!ft_strncmp(str + str_len - 3, ".rt", 3))
		return (1);
	return (0);
}
