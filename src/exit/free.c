/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:33:11 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 13:48:11 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Utility functions for memory cleanup and thread management.                */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_bvh_nonleaf_and_geo(t_bvh *bvh)
{
	int	i;

	i = -1;
	while (bvh->children[++i] != BVH_CHILD_END)
	{
		if (bvh->children[i] != BVH_LEAF)
			free(bvh->group[i]);
		free(bvh->obj_geo[i]);
	}
}

void	free_bvh_leaf_and_struct(t_bvh *bvh)
{
	int	i;

	i = -1;
	while (bvh->children[++i] != BVH_CHILD_END)
	{
		if (bvh->children[i] == BVH_LEAF)
			free(bvh->group[i]);
	}
	free(bvh);
}

void	wait_for_render_threads(t_data *data)
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

void	cleanup_data_resources(t_data *data)
{
	free(data->all_objects);
	free(data->objects);
	free(data->lights);
	pthread_mutex_destroy(&data->joblist_mutex);
}

void	exit_with_obj_parser_cleanup(t_obj_parser *parsers, int n_files)
{
	int	i;

	i = -1;
	while (++i < n_files)
		free(parsers[i].filename);
	free(parsers);
	exit(EXIT_FAILURE);
}
