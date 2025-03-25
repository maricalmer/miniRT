/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:53:28 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/24 10:47:12 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	get_pos_float(char **specs, float *size)
{
	if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	*size = ft_strtof(*specs, specs);
	if (errno == ERANGE || *size < 0)
		return (EXIT_FAILURE);
	if (**specs != ' ')
		return (EXIT_FAILURE);
	(*specs)++;
	return (EXIT_SUCCESS);
}
