#include "minirt.h"

static void	ft_sort_int_tab(__m256 input, char res[9][5]);


// __m256 aabb_test_SIMD

void aabb_test(t_bvh *bvh, int idx, float dir[3], float src[3], char res[9][5]) 
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
    ft_sort_int_tab(min, res);
}

// #include <immintrin.h>

// // **AVX1-Compatible Bitonic Merge for Floats**
// static inline __m256 bitonic_merge(__m256 a, __m256 b, __m256 mask) {
//     __m256 min_vals = _mm256_min_ps(a, b);
//     __m256 max_vals = _mm256_max_ps(a, b);
//     return _mm256_blendv_ps(min_vals, max_vals, mask);
// }

// // **Bitonic Sort for 8 Floats & Corresponding Indices (AVX1)**
// static void avx_bitonic_sort(__m256 *values, int *indices) {
//     float sorted_values[8];
//     int sorted_indices[8];

//     // Store values in an array
//     _mm256_storeu_ps(sorted_values, *values);

//     // Initialize indices
//     for (int i = 0; i < 8; i++) {
//         sorted_indices[i] = indices[i];
//     }

//     // **Perform CPU-side bitonic sort for indices**
//     for (int step = 2; step <= 8; step *= 2) {
//         for (int i = 0; i < 8; i += step) {
//             for (int j = 0; j < step / 2; j++) {
//                 int idx1 = i + j;
//                 int idx2 = i + j + (step / 2);
                
//                 if (sorted_values[idx1] > sorted_values[idx2]) {
//                     // Swap values
//                     float tmp_val = sorted_values[idx1];
//                     sorted_values[idx1] = sorted_values[idx2];
//                     sorted_values[idx2] = tmp_val;

//                     // Swap indices
//                     int tmp_idx = sorted_indices[idx1];
//                     sorted_indices[idx1] = sorted_indices[idx2];
//                     sorted_indices[idx2] = tmp_idx;
//                 }
//             }
//         }
//     }

//     // Store back sorted values & indices
//     *values = _mm256_loadu_ps(sorted_values);
//     for (int i = 0; i < 8; i++) {
//         indices[i] = sorted_indices[i];
//     }
// }


// AVX2 :
// static inline void bitonic_swap(__m256 *values, __m256i *indices, int mask)
// {
//     // Find the min and max values using the mask
//     __m256 min_vals = _mm256_min_ps(*values, _mm256_permutevar8x32_ps(*values, _mm256_set1_epi32(mask)));
//     __m256 max_vals = _mm256_max_ps(*values, _mm256_permutevar8x32_ps(*values, _mm256_set1_epi32(mask)));

//     // Find the corresponding min and max indices
//     __m256i min_indices = _mm256_blend_epi32(*indices, _mm256_permutevar8x32_epi32(*indices, _mm256_set1_epi32(mask)), 0b10101010);
//     __m256i max_indices = _mm256_blend_epi32(*indices, _mm256_permutevar8x32_epi32(*indices, _mm256_set1_epi32(mask)), 0b10101010);

//     // Swap the values and indices at the same time using the blend operation
//     *values = _mm256_blend_ps(min_vals, max_vals, 0b10101010);
//     *indices = _mm256_blend_epi32(min_indices, max_indices, 0b10101010);
// }

// 	// **AVX-optimized bitonic sort for 8 elements**
// 	static void avx_bitonic_sort(__m256 *values, __m256i *indices) {
// 		bitonic_swap(values, indices, 0b10110001);
// 		bitonic_swap(values, indices, 0b01001110);
// 		bitonic_swap(values, indices, 0b00011011);
// 	}

/* LEGACY */
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

static void	ft_sort_int_tab(__m256 input, char res[9][5])
{
	int	has_switched;
	int	i;
	float	values[8];
	float	tmp[8];
	int		indices[8];
	int 	size;

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

    // First, store indices of non -1 values
	int n = 0;
	i = 0;
	int j;
    while (n < size)
	{
		j = 0;
		res[i][j++] = indices[n++];
		while (n < size)
		{
			if (1)//values[n] == values[n - 1]) // account for precision here ! 
				res[i][j++] = indices[n++];
			else
				break;
		}
		res[i][j] = -1;
		i++;
	}
	res[i][0] = -1;
}

