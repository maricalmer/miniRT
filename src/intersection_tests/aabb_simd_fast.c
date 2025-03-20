/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_simd_fast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:22:11 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/03/19 23:59:33 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	ft_sort_indices(__m256 input, char res[9]);
static int	filter_valid_values(float *tmp, float *values, int *indices);
static void	handle_switch(float *values, int *indices, int size);
static int	switch_consecutive(float *tab1, int *tab2, int i);

void	aabb_test_fast(t_bvh *bvh, int idx, t_shoot *shoot, char res[9])
{
	__m256	min;

	min = aabb_test_simd(bvh, idx, shoot->dir, shoot->src);
	ft_sort_indices(min, res);
}

static void	ft_sort_indices(__m256 input, char res[9])
{
	float	values[8];
	float	tmp[8];
	int		indices[8];
	int		size;

	_mm256_storeu_ps(tmp, input);
	size = filter_valid_values(tmp, values, indices);
	handle_switch(values, indices, size);
	copy_and_terminate(res, indices, size);
}

static int	filter_valid_values(float *tmp, float *values, int *indices)
{
	int		i;
	int		size;

	i = -1;
	size = 0;
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

static void	handle_switch(float *values, int *indices, int size)
{
	int		has_switched;
	int		i;

	has_switched = 1;
	while (has_switched)
	{
		has_switched = 0;
		i = 0;
		while (i <= size - 2)
		{
			has_switched = (has_switched
					|| switch_consecutive(values, indices, i));
			i++;
		}
	}
}

static int	switch_consecutive(float *tab1, int *tab2, int i)
{
	int		temp;
	float	tmp;

	if (tab1[i] > tab1[i +1])
	{
		tmp = tab1[i];
		tab1[i] = tab1[i + 1];
		tab1[i + 1] = tmp;
		temp = tab2[i];
		tab2[i] = tab2[i + 1];
		tab2[i + 1] = temp;
		return (1);
	}
	else
		return (0);
}


