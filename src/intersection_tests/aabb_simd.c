/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_simd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:22:11 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/18 13:09:18 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//static void	test_axis_x(t_aabb_simd *x, t_bvh *bvh, int idx);
//static void	test_axis_y(t_aabb_simd *x, t_bvh *bvh, int idx);
//static void	test_axis_z(t_aabb_simd *x, t_bvh *bvh, int idx);
static void	test_axis_generic(t_aabb_simd *x,
	const float *min, const float *max);

__m256	aabb_test_simd(t_bvh *bvh, int idx, float dir[3], float src[3])
{
	t_aabb_simd	x;

	x.min = _mm256_set1_ps(0.0f);
	x.max = _mm256_set1_ps(1e10f);
	x.v_dir = _mm256_set1_ps(dir[0]);
	x.v_src = _mm256_set1_ps(src[0]);
	// test_axis_x(&x, bvh, idx);
    test_axis_generic(&x, &bvh->min_x[idx], &bvh->max_x[idx]);
	x.v_dir = _mm256_set1_ps(dir[1]);
	x.v_src = _mm256_set1_ps(src[1]);
	// test_axis_y(&x, bvh, idx);
    test_axis_generic(&x, &bvh->min_y[idx], &bvh->max_y[idx]);
	x.v_dir = _mm256_set1_ps(dir[2]);
	x.v_src = _mm256_set1_ps(src[2]);
	// test_axis_z(&x, bvh, idx);
    test_axis_generic(&x, &bvh->min_z[idx], &bvh->max_z[idx]);
	x.mask = _mm256_cmp_ps(x.min, x.max, _CMP_GT_OQ);
	x.min = _mm256_or_ps(_mm256_andnot_ps(x.mask, x.min),
			_mm256_and_ps(x.mask, _mm256_set1_ps(-1.0f)));
	return (x.min);
}

static void	test_axis_generic(t_aabb_simd *x, const float *min,
    const float *max)
{
	x->mask = _mm256_cmp_ps(x->v_dir, _mm256_set1_ps(0.0f), _CMP_EQ_OQ);
	x->odd = _mm256_rcp_ps(x->v_dir);
	x->bbox = _mm256_load_ps(min);
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox, _CMP_GE_OQ));
	x->t1 = _mm256_mul_ps(_mm256_sub_ps(x->bbox, x->v_src), x->odd);
	x->bbox = _mm256_load_ps(max);
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox, _CMP_LE_OQ));
	x->t2 = _mm256_mul_ps(_mm256_sub_ps(x->bbox, x->v_src), x->odd);
	x->tmp = x->t1;
	x->t1 = _mm256_min_ps(x->tmp, x->t2);
	x->t2 = _mm256_max_ps(x->tmp, x->t2);
	x->t1 = _mm256_andnot_ps(x->mask, x->t1);
	x->min = _mm256_max_ps(x->t1, x->min);
	x->max = _mm256_min_ps(x->t2, x->max);
}

/*
static void	test_axis_x(t_aabb_simd *x, t_bvh *bvh, int idx)
{
	x->mask = _mm256_cmp_ps(x->v_dir, _mm256_set1_ps(0.0f), _CMP_EQ_OQ);
	x->odd = _mm256_rcp_ps(x->v_dir);
	x->bbox = _mm256_load_ps(&bvh->min_x[idx]);
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox,
				_CMP_GE_OQ));
	x->t1 = _mm256_mul_ps(_mm256_sub_ps(x->bbox, x->v_src), x->odd);
	x->bbox = _mm256_load_ps(&bvh->max_x[idx]);
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox,
				_CMP_LE_OQ));
	x->t2 = _mm256_mul_ps(_mm256_sub_ps(x->bbox, x->v_src), x->odd);
	x->tmp = x->t1;
	x->t1 = _mm256_min_ps(x->tmp, x->t2);
	x->t2 = _mm256_max_ps(x->tmp, x->t2);
	x->t1 = _mm256_andnot_ps(x->mask, x->t1);
	x->min = _mm256_max_ps(x->t1, x->min);
	x->max = _mm256_min_ps(x->t2, x->max);
}

static void	test_axis_y(t_aabb_simd *x, t_bvh *bvh, int idx)
{
	x->mask = _mm256_cmp_ps(x->v_dir, _mm256_set1_ps(0.0f), _CMP_EQ_OQ);
	x->odd = _mm256_rcp_ps(x->v_dir);
	x->bbox = _mm256_load_ps(&bvh->min_y[idx]);
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox,
				_CMP_GE_OQ));
	x->t1 = _mm256_mul_ps(_mm256_sub_ps(x->bbox, x->v_src), x->odd);
	x->bbox = _mm256_load_ps(&bvh->max_y[idx]);
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox,
				_CMP_LE_OQ));
	x->t2 = _mm256_mul_ps(_mm256_sub_ps(x->bbox, x->v_src), x->odd);
	x->tmp = x->t1;
	x->t1 = _mm256_min_ps(x->tmp, x->t2);
	x->t2 = _mm256_max_ps(x->tmp, x->t2);
	x->t1 = _mm256_andnot_ps(x->mask, x->t1);
	x->min = _mm256_max_ps(x->t1, x->min);
	x->max = _mm256_min_ps(x->t2, x->max);
}

static void	test_axis_z(t_aabb_simd *x, t_bvh *bvh, int idx)
{
	x->mask = _mm256_cmp_ps(x->v_dir, _mm256_set1_ps(0.0f), _CMP_EQ_OQ);
	x->odd = _mm256_rcp_ps(x->v_dir);
	x->bbox = _mm256_load_ps(&bvh->min_z[idx]);
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox,
				_CMP_GE_OQ));
	x->t1 = _mm256_mul_ps(_mm256_sub_ps(x->bbox, x->v_src), x->odd);
	x->bbox = _mm256_load_ps(&bvh->max_z[idx]);
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox,
				_CMP_LE_OQ));
	x->t2 = _mm256_mul_ps(_mm256_sub_ps(x->bbox, x->v_src), x->odd);
	x->tmp = x->t1;
	x->t1 = _mm256_min_ps(x->tmp, x->t2);
	x->t2 = _mm256_max_ps(x->tmp, x->t2);
	x->t1 = _mm256_andnot_ps(x->mask, x->t1);
	x->min = _mm256_max_ps(x->t1, x->min);
	x->max = _mm256_min_ps(x->t2, x->max);
}
*/

void	copy_and_terminate(char *res, int *indices, int size)
{
	int	n;
	int	i;

	n = 0;
	i = 0;
	while (n < size)
		res[i++] = indices[n++];
	res[size] = -1;
}
