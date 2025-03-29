/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:49:05 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/29 15:54:00 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	get_coord(char **specs, float *value)
{
	int	i;

	i = -1;
	while (++i < 3)
	{
		if (!ft_isdigit(**specs) && **specs != '-')
			return (EXIT_FAILURE);
		value[i] = ft_strtof(*specs, specs);
		if (errno == ERANGE)
			return (EXIT_FAILURE);
		while (!ft_isdigit(**specs) && **specs != '\0' && **specs != '-')
		{
			if (i < 2 && **specs != ',')
				return (EXIT_FAILURE);
			if (i > 2 && **specs != ' ')
				return (EXIT_FAILURE);
			(*specs)++;
		}
	}
	return (EXIT_SUCCESS);
}

int	get_vec_norm(char **specs, float *value)
{
	int	i;

	i = -1;
	while (++i < 3)
	{
		if (!ft_isdigit(**specs) && **specs != '-')
			return (EXIT_FAILURE);
		value[i] = ft_strtof(*specs, specs);
		if (errno == ERANGE || (value[i] < -1.0 || value[i] > 1.0))
			return (EXIT_FAILURE);
		while (!ft_isdigit(**specs) && **specs != '\0' && **specs != '-')
		{
			if (i < 2 && **specs != ',')
				return (EXIT_FAILURE);
			if (i > 2 && **specs != ' ')
				return (EXIT_FAILURE);
			(*specs)++;
		}
	}
	normalize(value, NULL);
	return (EXIT_SUCCESS);
}

int	get_radius(char **specs, float *radius)
{
	if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	*radius = ft_strtof(*specs, specs) * 0.5f;
	if (errno == ERANGE)
		return (EXIT_FAILURE);
	if (**specs != ' ')
		return (EXIT_FAILURE);
	(*specs)++;
	return (EXIT_SUCCESS);
}

int	get_length(char **specs, float *length)
{
	if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	*length = ft_strtof(*specs, specs);
	if (errno == ERANGE)
		return (EXIT_FAILURE);
	if (**specs != ' ')
		return (EXIT_FAILURE);
	(*specs)++;
	return (EXIT_SUCCESS);
}

int	get_fov_range(char **specs, int *fov)
{
	if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	*fov = ft_strtoi(*specs, specs);
	if (errno == ERANGE || (*fov < 0 || *fov > 180))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
