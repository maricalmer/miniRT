/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_simd_fast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:22:11 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 14:30:17 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Fast AABB test for BVH traversal using SIMD. Filters out invalid           */
/* intersections early (= -1). Sorts valid hits front-to-back for optimal     */
/* traversal. Helps early-exit and reduces expensive geometry tests. Most     */
/* effective in complex scenes with convex volumes (e.g. suzanne). Uses AVX   */
/* to process 8 AABBs in parallel. Returns sorted valid indices in `res`.     */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	sort_indices(__m256 input, char res[9]);
static int	filter_valid_values(float *tmp, float *values, int *indices);
static void	bubble_sort_values_indices(float *values, int *indices, int size);
static int	switch_consecutive(float *values, int *indices, int i);

void	aabb_test_fast(t_bvh *bvh, int idx, t_shoot *shoot, char res[9])
{
	__m256	min;

	min = aabb_test_simd(bvh, idx, shoot->dir, shoot->src);
	sort_indices(min, res);
}

static void	sort_indices(__m256 input, char res[9])
{
	float	values[8];
	float	tmp[8];
	int		indices[8];
	int		size;

	_mm256_storeu_ps(tmp, input);
	size = filter_valid_values(tmp, values, indices);
	bubble_sort_values_indices(values, indices, size);
	copy_and_terminate(res, indices, size);
}

static int	filter_valid_values(float *tmp, float *values, int *indices)
{
	int		i;
	int		size;

	size = 0;
	i = -1;
	while (++i < 8)
	{
		if (tmp[i] != -1)
		{
			values[size] = tmp[i];
			indices[size] = i;
			size++;
		}
	}
	return (size);
}

static void	bubble_sort_values_indices(float *values, int *indices, int size)
{
	int		has_switched;
	int		i;

	has_switched = 1;
	while (has_switched)
	{
		has_switched = 0;
		i = -1;
		while (++i <= size - 2)
		{
			has_switched = (has_switched
					|| switch_consecutive(values, indices, i));
		}
	}
}

static int	switch_consecutive(float *values, int *indices, int i)
{
	int		temp;
	float	tmp;

	if (values[i] > values[i + 1])
	{
		tmp = values[i];
		values[i] = values[i + 1];
		values[i + 1] = tmp;
		temp = indices[i];
		indices[i] = indices[i + 1];
		indices[i + 1] = temp;
		return (1);
	}
	else
		return (0);
}
