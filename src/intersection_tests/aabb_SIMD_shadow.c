#include "minirt.h"

static void	ft_sort_int_tab(__m256 t_min, __m256 t_max, char res[9]);

void aabb_test_shadow(t_bvh *bvh, int idx, float dir[3], float src[3], char res[9]) 
{
    __m256 bbox;
    __m256 v_dir;
    __m256 v_src;
    __m256 odd;
    __m256 tmp;

    __m256 t1;
    __m256 t2;

    __m256 mask;
    __m256 min = _mm256_set1_ps(0.0f);
    __m256 max = _mm256_set1_ps(1e10f);

	//x
	v_dir = _mm256_set1_ps(dir[0]);
	v_src = _mm256_set1_ps(src[0]);

	mask = _mm256_cmp_ps(v_dir, _mm256_set1_ps(0.0f), _CMP_EQ_OQ);

	odd = _mm256_rcp_ps(v_dir);

	bbox = _mm256_load_ps(&bvh->min_x[idx]);
	mask = _mm256_and_ps(mask, _mm256_cmp_ps(v_src, bbox, _CMP_GE_OQ)); // inside the box
	t1 = _mm256_mul_ps(_mm256_sub_ps(bbox, v_src), odd);

	bbox = _mm256_load_ps(&bvh->max_x[idx]);
	mask = _mm256_and_ps(mask, _mm256_cmp_ps(v_src, bbox, _CMP_LE_OQ)); // inside the box
	t2 = _mm256_mul_ps(_mm256_sub_ps(bbox, v_src), odd);

	tmp = t1;
	t1 = _mm256_min_ps(tmp, t2);
	t2 = _mm256_max_ps(tmp, t2);

	t1 = _mm256_andnot_ps(mask, t1); // set t1=0 if parallel and inside the box :)
	// t2 is implicit as it will be BIG....

	min = _mm256_max_ps(t1, min);
	max = _mm256_min_ps(t2, max);
    
	//y
        v_dir = _mm256_set1_ps(dir[1]);
        v_src = _mm256_set1_ps(src[1]);

        mask = _mm256_cmp_ps(v_dir, _mm256_set1_ps(0.0f), _CMP_EQ_OQ);

        odd = _mm256_rcp_ps(v_dir);

        bbox = _mm256_load_ps(&bvh->min_y[idx]);
        mask = _mm256_and_ps(mask, _mm256_cmp_ps(v_src, bbox, _CMP_GE_OQ)); // inside the box
        t1 = _mm256_mul_ps(_mm256_sub_ps(bbox, v_src), odd);

        bbox = _mm256_load_ps(&bvh->max_y[idx]);
        mask = _mm256_and_ps(mask, _mm256_cmp_ps(v_src, bbox, _CMP_LE_OQ)); // inside the box
        t2 = _mm256_mul_ps(_mm256_sub_ps(bbox, v_src), odd);

        tmp = t1;
        t1 = _mm256_min_ps(tmp, t2);
        t2 = _mm256_max_ps(tmp, t2);

        t1 = _mm256_andnot_ps(mask, t1); // set t1=0 if parallel and inside the box :)
        // t2 is implicit as it will be BIG....

        min = _mm256_max_ps(t1, min);
        max = _mm256_min_ps(t2, max);

	//z
        v_dir = _mm256_set1_ps(dir[2]);
        v_src = _mm256_set1_ps(src[2]);

        mask = _mm256_cmp_ps(v_dir, _mm256_set1_ps(0.0f), _CMP_EQ_OQ);

        odd = _mm256_rcp_ps(v_dir);

        bbox = _mm256_load_ps(&bvh->min_z[idx]);
        mask = _mm256_and_ps(mask, _mm256_cmp_ps(v_src, bbox, _CMP_GE_OQ)); // inside the box
        t1 = _mm256_mul_ps(_mm256_sub_ps(bbox, v_src), odd);

        bbox = _mm256_load_ps(&bvh->max_z[idx]);
        mask = _mm256_and_ps(mask, _mm256_cmp_ps(v_src, bbox, _CMP_LE_OQ)); // inside the box
        t2 = _mm256_mul_ps(_mm256_sub_ps(bbox, v_src), odd);

        tmp = t1;
        t1 = _mm256_min_ps(tmp, t2);
        t2 = _mm256_max_ps(tmp, t2);

        t1 = _mm256_andnot_ps(mask, t1); // set t1=0 if parallel and inside the box :)
        // t2 is implicit as it will be BIG....

        min = _mm256_max_ps(t1, min);
        max = _mm256_min_ps(t2, max);
    
    mask = _mm256_cmp_ps(min, max, _CMP_GT_OQ);
    min = _mm256_or_ps(_mm256_andnot_ps(mask, min), _mm256_and_ps(mask, _mm256_set1_ps(-1.0f))); // Replace _mm256_blendv_ps with bitwise operations
    ft_sort_int_tab(min, max, res);
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

static void	ft_sort_int_tab(__m256 t_min, __m256 t_max, char res[9])
{
	int	has_switched;
	int	i;
	float	values[8];
	float	tmp[8];
	float	tmp2[8];
	int		indices[8];
	int 	size;

	_mm256_storeu_ps(tmp, t_min);
	_mm256_storeu_ps(tmp2, t_max);

	i = -1;
	size = 0;
	while (++i < 8)
	{	
		if (tmp[i] != -1)
		{
			values[size] = tmp2[i] - tmp[i];
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

    // sort the indice based on max distance travelled in the box
	i = - 1;
	while (++i < size)
	{
		res[i] = indices[size - 1 - i];
	}
	// if (size < 8)
	res[size] = -1;
}
