#include "minirt.h"

t_intersect_result	intersection_test_aabb(t_aabb *aabb, float dir[3], float src[3])
{
	t_intersect_result	t;
	float				ood[3];
	float				t1[3];
	float				t2[3];
	int					i;
	int					break_it;
	int					dir_positif;

	t.min = 0;
	t.max = FLT_MAX;
	break_it = 0;
	i = -1;
	while (++i < 3)
		break_it += fabsf(dir[i]) < EPSILON && (src[i] < aabb->pt_min[i] || src[i] > aabb->pt_max[i]);
	if (break_it)
	{
		t.min = -1;
		return (t);
	}
	i = -1;
	while (++i < 3)
	{
		ood[i] = 1.0 / dir[i];
	}
	
	dir_positif = ((dir[0] > 0) << 2) | ((dir[1] > 0) << 1) | (dir[2] > 0);
	if (dir_positif == 0b111)
	{
			t1[0] = (aabb->pt_min[0] - src[0]) * ood[0];
			t1[1] = (aabb->pt_min[1] - src[1]) * ood[1];
			t1[2] = (aabb->pt_min[2] - src[2]) * ood[2];
			t2[0] = (aabb->pt_max[0] - src[0]) * ood[0];
			t2[1] = (aabb->pt_max[1] - src[1]) * ood[1];
			t2[2] = (aabb->pt_max[2] - src[2]) * ood[2];
	}
	else if (dir_positif == 0b110)
	{
			t1[0] = (aabb->pt_min[0] - src[0]) * ood[0];
			t1[1] = (aabb->pt_min[1] - src[1]) * ood[1];
			t1[2] = (aabb->pt_max[2] - src[2]) * ood[2];
			t2[0] = (aabb->pt_max[0] - src[0]) * ood[0];
			t2[1] = (aabb->pt_max[1] - src[1]) * ood[1];
			t2[2] = (aabb->pt_min[2] - src[2]) * ood[2];
	}
	else if (dir_positif == 0b101)
	{
			t1[0] = (aabb->pt_min[0] - src[0]) * ood[0];
			t1[1] = (aabb->pt_max[1] - src[1]) * ood[1];
			t1[2] = (aabb->pt_min[2] - src[2]) * ood[2];
			t2[0] = (aabb->pt_max[0] - src[0]) * ood[0];
			t2[1] = (aabb->pt_min[1] - src[1]) * ood[1];
			t2[2] = (aabb->pt_max[2] - src[2]) * ood[2];
	}

	else if (dir_positif == 0b011)
	{
			t1[0] = (aabb->pt_max[0] - src[0]) * ood[0];
			t1[1] = (aabb->pt_min[1] - src[1]) * ood[1];
			t1[2] = (aabb->pt_min[2] - src[2]) * ood[2];
			t2[0] = (aabb->pt_min[0] - src[0]) * ood[0];
			t2[1] = (aabb->pt_max[1] - src[1]) * ood[1];
			t2[2] = (aabb->pt_max[2] - src[2]) * ood[2];
	}

	else if (dir_positif == 0b001)
	{
			t1[0] = (aabb->pt_max[0] - src[0]) * ood[0];
			t1[1] = (aabb->pt_max[1] - src[1]) * ood[1];
			t1[2] = (aabb->pt_min[2] - src[2]) * ood[2];
			t2[0] = (aabb->pt_min[0] - src[0]) * ood[0];
			t2[1] = (aabb->pt_min[1] - src[1]) * ood[1];
			t2[2] = (aabb->pt_max[2] - src[2]) * ood[2];
	}

	else if (dir_positif == 0b100)
	{
			t1[0] = (aabb->pt_min[0] - src[0]) * ood[0];
			t1[1] = (aabb->pt_max[1] - src[1]) * ood[1];
			t1[2] = (aabb->pt_max[2] - src[2]) * ood[2];
			t2[0] = (aabb->pt_max[0] - src[0]) * ood[0];
			t2[1] = (aabb->pt_min[1] - src[1]) * ood[1];
			t2[2] = (aabb->pt_min[2] - src[2]) * ood[2];
	}
	
	else if (dir_positif == 0b010)
	{
			t1[0] = (aabb->pt_max[0] - src[0]) * ood[0];
			t1[1] = (aabb->pt_min[1] - src[1]) * ood[1];
			t1[2] = (aabb->pt_max[2] - src[2]) * ood[2];
			t2[0] = (aabb->pt_min[0] - src[0]) * ood[0];
			t2[1] = (aabb->pt_max[1] - src[1]) * ood[1];
			t2[2] = (aabb->pt_min[2] - src[2]) * ood[2];
	}

	else // (dir_positif == 0b000)
	{
			t1[0] = (aabb->pt_max[0] - src[0]) * ood[0];
			t1[1] = (aabb->pt_max[1] - src[1]) * ood[1];
			t1[2] = (aabb->pt_max[2] - src[2]) * ood[2];
			t2[0] = (aabb->pt_min[0] - src[0]) * ood[0];
			t2[1] = (aabb->pt_min[1] - src[1]) * ood[1];
			t2[2] = (aabb->pt_min[2] - src[2]) * ood[2];
	}
	t.min = fmaxf(t.min, fmaxf(t1[0], fmaxf(t1[1], t1[2])));
	t.max = fminf(t.max, fminf(t2[0], fminf(t2[1], t2[2])));
	if (t.min > t.max)
	{
		t.min = -1;
		return (t);
	}
	return (t);
}


t_intersect_result	intersection_test_aabb_avx(t_aabb *aabb, float dir[3], float src[3])
{
    t_intersect_result t = { 0.0f, FLT_MAX };

    // Load dir, src, pt_min, and pt_max into AVX registers
    // __m128 v_dir = _mm_loadu_ps(dir);      // dir[x], dir[y], dir[z], garbage
    // __m128 v_src = _mm_loadu_ps(src);      // src[x], src[y], src[z], garbage
    // __m128 v_min = _mm_loadu_ps(aabb->pt_min); // pt_min[x], pt_min[y], pt_min[z], garbage
    // __m128 v_max = _mm_loadu_ps(aabb->pt_max); // pt_max[x], pt_max[y], pt_max[z], garbage
	__m128 v_dir = _mm_setr_ps(dir[0], dir[1], dir[2], 0.0f);
	__m128 v_src = _mm_setr_ps(src[0], src[1], src[2], 0.0f);
	__m128 v_min = _mm_setr_ps(aabb->pt_min[0], aabb->pt_min[1], aabb->pt_min[2], 0.0f);
	__m128 v_max = _mm_setr_ps(aabb->pt_max[0], aabb->pt_max[1], aabb->pt_max[2], 0.0f);


    // Compute absolute dir and check for near-zero values (parallel check)
    __m128 v_abs_dir = _mm_andnot_ps(_mm_set1_ps(-0.0f), v_dir); // fabs(dir)
    __m128 v_near_zero = _mm_cmplt_ps(v_abs_dir, _mm_set1_ps(1e-8f)); // Check if dir[i] < EPSILON

    // Check if src is outside AABB for parallel rays
    __m128 v_outside_min = _mm_cmplt_ps(v_src, v_min);
    __m128 v_outside_max = _mm_cmpgt_ps(v_src, v_max);
    __m128 v_fail = _mm_and_ps(_mm_or_ps(v_outside_min, v_outside_max), v_near_zero); // Fail if parallel and outside

    // If any component fails, return immediately
    if (_mm_movemask_ps(v_fail)) {
        t.min = -1.0f;
        return t;
    }

    // Compute reciprocal (1.0 / dir) where dir is not zero
    __m128 v_ood = _mm_blendv_ps(_mm_rcp_ps(v_dir), _mm_setzero_ps(), v_near_zero);

    // Compute intersection distances
    __m128 v_t1 = _mm_mul_ps(_mm_sub_ps(v_min, v_src), v_ood);
    __m128 v_t2 = _mm_mul_ps(_mm_sub_ps(v_max, v_src), v_ood);

    // Ensure t1 <= t2 (swap where necessary)
    __m128 v_tmin = _mm_min_ps(v_t1, v_t2);
    __m128 v_tmax = _mm_max_ps(v_t1, v_t2);

    // Compute final min/max values
    t.min = fmaxf(fmaxf(t.min, _mm_cvtss_f32(v_tmin)), fmaxf(_mm_cvtss_f32(_mm_shuffle_ps(v_tmin, v_tmin, _MM_SHUFFLE(1, 1, 1, 1))),
                                                               _mm_cvtss_f32(_mm_shuffle_ps(v_tmin, v_tmin, _MM_SHUFFLE(2, 2, 2, 2)))));

    t.max = fminf(fminf(t.max, _mm_cvtss_f32(v_tmax)), fminf(_mm_cvtss_f32(_mm_shuffle_ps(v_tmax, v_tmax, _MM_SHUFFLE(1, 1, 1, 1))),
                                                               _mm_cvtss_f32(_mm_shuffle_ps(v_tmax, v_tmax, _MM_SHUFFLE(2, 2, 2, 2)))));

    // If t.min > t.max, return failure
    if (t.min > t.max)
        t.min = -1.0f;

    return t;
}



t_intersect_result	intersection_test_aabb_legacy(t_aabb *aabb, float dir[3], float src[3])
{
	t_intersect_result	t;
	float				ood;
	float				t1;
	float				t2;
	int					i;

	t.min = 0;
	t.max = FLT_MAX;
	i = -1;
	while (++i < 3)
	{
		if (fabsf(dir[i]) < EPSILON)
		{
			if (src[i] < aabb->pt_min[i] || src[i] > aabb->pt_max[i])
			{
				t.min = -1;
				return (t);
			}
		}
		else
		{
			ood = 1.0 / dir[i];
			t1 = (aabb->pt_min[i] - src[i]) * ood;
			t2 = (aabb->pt_max[i] - src[i]) * ood;
			if (t1 > t2)
				ft_swap(&t1, &t2);
			t.min = fmaxf(t.min, t1);
			t.max = fminf(t.max, t2);
			if (t.min > t.max)
			{
				t.min = -1;
				return (t);
			}
		}
	}
	return (t);
}
