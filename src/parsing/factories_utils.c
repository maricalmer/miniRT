/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:25:58 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/23 10:19:27 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	get_ratio(char **specs, float *ratio)
{
	if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	*ratio = ft_strtof(*specs, specs);
	if (errno == ERANGE || (*ratio < 0.0 || *ratio > 1.0))
		return (EXIT_FAILURE);
	if (**specs != ' ')
		return (EXIT_FAILURE);
	(*specs)++;
	return (EXIT_SUCCESS);
}

int	get_rgb_norm(char **specs, float *color)
{
	int	i;

	i = -1;
	while (++i < 3)
	{
		if (!ft_isdigit(**specs))
			return (EXIT_FAILURE);
		color[i] = ft_strtof(*specs, specs) / 255.0f;
		if (errno == ERANGE || (color[i] < 0.0 || color[i] > 1.0))
			return (EXIT_FAILURE);
		while (!ft_isdigit(**specs) && **specs != '\0')
		{
			if (**specs != ' ' && **specs != ',')
				return (EXIT_FAILURE);
			(*specs)++;
		}
	}
	return (EXIT_SUCCESS);
}

int	get_rgb(char **specs, unsigned char *color)
{
	int	i;

	i = -1;
	while (++i < 3)
	{
		if (!ft_isdigit(**specs))
			return (EXIT_FAILURE);
		color[i] = ft_strtoi(*specs, specs);
		if (errno == ERANGE || (color[i] < 0 || color[i] > 255))
			return (EXIT_FAILURE);
		while (!ft_isdigit(**specs) && **specs != '\0')
		{
			if (**specs != ' ' && **specs != ',')
				return (EXIT_FAILURE);
			(*specs)++;
		}
	}
	return (EXIT_SUCCESS);
}

int	get_obj_rgb(char **specs, int *color)
{
	if (!ft_isdigit(**specs) && **specs != '-')
		return (EXIT_FAILURE);
	*color = ft_strtoi(*specs, specs);
	if (errno == ERANGE || (*color < -1 || *color > 255))
		return (EXIT_FAILURE);
	while (!ft_isdigit(**specs) && **specs != '\0' && **specs != '-')
	{
		if (**specs != ' ' && **specs != ',')
			return (EXIT_FAILURE);
		(*specs)++;
	}
	return (EXIT_SUCCESS);
}

int	get_refr_idx(char **specs, float *ratio)
{
	char	*p;

	if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	p = *specs;
	*ratio = ft_strtof(*specs, specs);
	if (errno == ERANGE || (*ratio < 0.0 || *ratio > 2.7))
		return (EXIT_FAILURE);
	if (**specs != ' ' && *specs != p)
		return (EXIT_FAILURE);
	if (**specs == ' ')
		(*specs)++;
	return (EXIT_SUCCESS);
}
