/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 09:53:28 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/23 22:34:31 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Parses a positive float value from a string ensuring it is valid. Performs */
/* error checks and moves the string pointer forward after parsing.           */
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
