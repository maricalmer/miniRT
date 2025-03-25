/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:33:11 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/25 21:09:26 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_data(t_data *data)
{
	// free(data->all_objects);  creates invalid free in valgrind
	free(data->objects);
	free(data->lights);
}

