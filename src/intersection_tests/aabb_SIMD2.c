#include "minirt.h"

typedef struct
{
    __m256 bbox;
    __m256 v_dir;
    __m256 v_src;
    __m256 odd;
    __m256 tmp;

    __m256 t1;
    __m256 t2;

    __m256 mask;
    __m256 min;
    __m256 max;
}	t_aabb_simd;

static void	ft_sort_indices(__m256 input, char res[9]);
static void	test_axis_x(t_aabb_simd *x, t_bvh *bvh, int idx);
static void	test_axis_y(t_aabb_simd *x, t_bvh *bvh, int idx);
static void	test_axis_z(t_aabb_simd *x, t_bvh *bvh, int idx);

__m256 aabb_test_SIMD(t_bvh *bvh, int idx, float dir[3], float src[3])
{
	t_aabb_simd x;
	
	x.min = _mm256_set1_ps(0.0f);
    x.max = _mm256_set1_ps(1e10f);
	x.v_dir = _mm256_set1_ps(dir[0]);
	x.v_src = _mm256_set1_ps(src[0]);
	test_axis_x(&x, bvh, idx);
	x.v_dir = _mm256_set1_ps(dir[1]);
	x.v_src = _mm256_set1_ps(src[1]);
	test_axis_y(&x, bvh, idx);
	x.v_dir = _mm256_set1_ps(dir[2]);
	x.v_src = _mm256_set1_ps(src[2]);
	test_axis_z(&x, bvh, idx);
	x.mask = _mm256_cmp_ps(x.min, x.max, _CMP_GT_OQ);
    x.min = _mm256_or_ps(_mm256_andnot_ps(x.mask, x.min),
			_mm256_and_ps(x.mask, _mm256_set1_ps(-1.0f)));
	return(x.min);
}

static void	test_axis_x(t_aabb_simd *x, t_bvh *bvh, int idx)
{
	x->mask = _mm256_cmp_ps(x->v_dir, _mm256_set1_ps(0.0f), _CMP_EQ_OQ);
	x->odd = _mm256_rcp_ps(x->v_dir);
	x->bbox = _mm256_load_ps(&bvh->min_x[idx]);
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox, _CMP_GE_OQ));
	x->t1 = _mm256_mul_ps(_mm256_sub_ps(x->bbox, x->v_src), x->odd);
	x->bbox = _mm256_load_ps(&bvh->max_x[idx]);
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox, _CMP_LE_OQ));
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
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox, _CMP_GE_OQ));
	x->t1 = _mm256_mul_ps(_mm256_sub_ps(x->bbox, x->v_src), x->odd);
	x->bbox = _mm256_load_ps(&bvh->max_y[idx]);
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox, _CMP_LE_OQ));
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
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox, _CMP_GE_OQ));
	x->t1 = _mm256_mul_ps(_mm256_sub_ps(x->bbox, x->v_src), x->odd);
	x->bbox = _mm256_load_ps(&bvh->max_z[idx]);
	x->mask = _mm256_and_ps(x->mask, _mm256_cmp_ps(x->v_src, x->bbox, _CMP_LE_OQ));
	x->t2 = _mm256_mul_ps(_mm256_sub_ps(x->bbox, x->v_src), x->odd);
	x->tmp = x->t1;
	x->t1 = _mm256_min_ps(x->tmp, x->t2);
	x->t2 = _mm256_max_ps(x->tmp, x->t2);
	x->t1 = _mm256_andnot_ps(x->mask, x->t1);
	x->min = _mm256_max_ps(x->t1, x->min);
	x->max = _mm256_min_ps(x->t2, x->max);
}

void aabb_test_fast(t_bvh *bvh, int idx, float dir[3], float src[3], char res[9]) 
{
    __m256	min;

	min = aabb_test_SIMD(bvh, idx, dir, src);
	ft_sort_indices(min, res);
}

static int	switch_consecutive(float *tab1, int *tab2, int i)
{
	int	temp;
	float tmp;

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

static void	ft_sort_indices(__m256 input, char res[9])
{
	int		has_switched;
	int		i;
	float	values[8];
	float	tmp[8];
	int		indices[8];
	int 	size;
	int 	n;

	_mm256_storeu_ps(tmp, input);
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
	has_switched = 1;
	while (has_switched)
	{
		has_switched = 0;
		i = 0;
		while (i <= size - 2)
		{
			has_switched = (has_switched || switch_consecutive(values, indices, i));
			i++;
		}
	}
	n = 0;
	i = 0;
    while (n < size)
		res[i++] = indices[n++];
	res[size] = -1;
}

