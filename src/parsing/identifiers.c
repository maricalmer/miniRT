/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifiers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:59:12 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 19:18:19 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Utility functions. Identifies different scene elements based on the prefix */
/* of their specification strings.                                            */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	is_object_file(char *specs)
{
	return (specs[0] == 'o' && specs[1] == ' ');
}

int	is_light(char *specs)
{
	return (specs[0] == 'L' && specs[1] == ' ');
}

int	is_cam(char *specs)
{
	return (specs[0] == 'C' && specs[1] == ' ');
}

int	is_ambient(char *specs)
{
	return (specs[0] == 'A' && specs[1] == ' ');
}

int	is_plane(char *specs)
{
	return (specs[0] == 'p' && specs[1] == 'l' && specs[2] == ' ');
}
