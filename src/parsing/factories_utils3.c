/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:53:28 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/23 13:57:48 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	get_checkerboard_flag(char **specs, int *flag)
{
	if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	*flag = ft_strtoi(*specs, specs);
	if (errno == ERANGE || (*flag != 0 && *flag != 1))
		return (EXIT_FAILURE);
	if (**specs != ' ')
		return (EXIT_FAILURE);
	(*specs)++;
	return (EXIT_SUCCESS);
}
