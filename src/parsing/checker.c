/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:25:18 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/15 18:33:13 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	is_rt_extension(char *str);
static void	check_filename(char *file, int *fd);

void	check_input(int ac, char **av, t_data *data)
{
	if (ac != 2)
	{
		print_error(1);
		exit(EXIT_FAILURE);
	}
	check_filename(av[1], &data->rt_fd); 
}

static void	check_filename(char *file, int *fd)
{
	if (!ft_strncmp(file, "", 1) || !is_rt_extension(file))
	{
		print_error(2);
		exit(EXIT_FAILURE);
	}
	*fd = open(file, O_RDONLY);
	if (*fd < 0)
		handle_file_error(__func__, file);
}

static int	is_rt_extension(char *str)
{
	int	str_len;

	str_len = ft_strlen(str);
	if (str_len < 4)
		return (0);
	if (!ft_strncmp(str + str_len - 3, ".rt", 3))
		return (1);
	return (0);
}
