/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:37:58 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/23 13:38:28 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_obj_parse_2(t_obj_parser *parsers, int n_files)
{
	int	i;

	i = -1;
	while (++i < n_files)
	{
		free(parsers[i].filename);
		free(parsers[i].vertices);
		free(parsers[i].normals);
	}
	free(parsers);
}
