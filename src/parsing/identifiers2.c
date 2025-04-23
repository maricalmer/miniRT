/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifiers2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:59:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/23 22:57:35 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Utility functions. Identifies different scene elements based on the prefix */
/* of their specification strings.                                            */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	is_sphere(char *specs)
{
	return (specs[0] == 's' && specs[1] == 'p' && specs[2] == ' ');
}

int	is_cylinder(char *specs)
{
	return (specs[0] == 'c' && specs[1] == 'y' && specs[2] == ' ');
}

int	is_face(char *specs)
{
	return (specs[0] == 'f' && specs[1] == ' ');
}

int	is_smoothing(char *specs)
{
	return (specs[0] == 's' && specs[1] == ' ');
}

int	is_rectangle(char *specs)
{
	return (specs[0] == 'r' && specs[1] == 'e' && specs[2] == ' ');
}
