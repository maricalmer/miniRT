/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:33:11 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/18 01:11:39 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_bvh_1(t_bvh *bvh)
{
	int	i;

	i = -1;
	while (bvh->childs[++i] != BVH_CHILD_END)
	{
		if (bvh->childs[i] != BVH_LEAF)
			free(bvh->group[i]);
		free(bvh->obj_geo[i]);
	}
}

void	free_bvh_2(t_bvh *bvh)
{
	int	i;

	i = -1;
	while (bvh->childs[++i] != BVH_CHILD_END)
	{
		if (bvh->childs[i] == BVH_LEAF)
			free(bvh->group[i]);
	}
	free(bvh);
}

void	join_threads(t_data *data)
{
	int	i;

	data->exit = 1;
	i = -1;
	while (++i < N_THREAD)
	{
		if (pthread_join(data->threads[i], NULL))
			exit(EXIT_FAILURE);
	}
}

void	free_data(t_data *data)
{
	free(data->all_objects);
	free(data->objects);
	free(data->lights);
	pthread_mutex_destroy(&data->joblist_mutex);
}

void	free_obj_parse_1_and_exit(t_obj_parser *parsers, int n_files)
{
	int	i;

	i = -1;
	while (++i < n_files)
		free(parsers[i].filename);
	free(parsers);
	exit(EXIT_FAILURE);
}
