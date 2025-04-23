/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:39:21 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/23 19:37:26 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Logs timing information and mesh statistics. Reports BVH build time,       */
/* per-image render times, and total triangle count for loaded triangle       */
/* meshes.                                                                    */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	print_bvh_build_t(struct timeval t_start, struct timeval t_end)
{
	float	t;

	t = (t_end.tv_sec - t_start.tv_sec) * 1000
		+ (t_end.tv_usec - t_start.tv_usec) * 0.001; 
	printf("%s    > %.2f ms to build BVH\n", CYAN_TXT_START, t);
	printf("\n  [RENDER]%s\n", COLOR_END);
}

void	print_img_render_t(struct timeval t_start, struct timeval t_end)
{
	static int		n_img;
	float			t;

	t = (t_end.tv_sec - t_start.tv_sec) * 1000
		+ (t_end.tv_usec - t_start.tv_usec) * 0.001; 
	printf("%s    > img %d rendered after %.1f ms%s\n", CYAN_TXT_START,
		n_img++, t, COLOR_END);
}

void	print_tri_count(int counter)
{
	printf("%s  [TRIMESH]\n\n", CYAN_TXT_START);
	printf("    > %d tris%s\n\n", counter, COLOR_END);
}
