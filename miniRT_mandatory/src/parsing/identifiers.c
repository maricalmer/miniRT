/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifiers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:59:12 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/25 15:06:37 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


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
