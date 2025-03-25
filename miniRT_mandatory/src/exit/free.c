/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:33:11 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/25 11:55:38 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"



void	free_data(t_data *data)
{
	free(data->all_objects);
	free(data->objects);
	free(data->lights);
}

void	free_obj_parse_1(t_obj_parser *parsers, int n_files)
{
	int	i;

	i = -1;
	while (++i < n_files)
		free(parsers[i].filename);
	free(parsers);
}
