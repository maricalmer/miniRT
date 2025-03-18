#include "minirt.h"

float	visi_test_bvh_strict(t_bvh *bvh, int idx, t_shoot *shoot)
{
	__m256		v_res;
	float		res[8];
	int			i;
	float		t;
	float		t_min;
	t_object 	*obj;

	if (bvh->childs[idx] == -1)
		return (visi_test_leafs(bvh->group[idx], shoot, bvh->group_size[idx]));
	v_res = aabb_test_simd(bvh, bvh->childs[idx], shoot->dir, shoot->src);
	_mm256_storeu_ps(res, v_res);
	i = -1;
	t_min = FLT_MAX;
	while (++i < 8)
	{
		if (res[i] != -1)
		{
			t = visi_test_bvh_strict(bvh, bvh->childs[idx] + i, shoot);
			if (t > 0 && t < t_min)
			{
				t_min = t;
				obj = shoot->obj;
			}
		}
	}
	if (t_min != FLT_MAX)
	{
		shoot->obj = obj;
		return (t_min);
	}
	return (0);
}

float	visi_test_bvh_fast(t_bvh *bvh, int idx, t_shoot *shoot)
{
	char		res[9];
	int			i;
	float		t;

	if (bvh->childs[idx] == -1)
		return (visi_test_leafs(bvh->group[idx], shoot, bvh->group_size[idx]));
	aabb_test_fast(bvh, bvh->childs[idx], shoot, res);
	i = -1;
	while (res[++i] != -1)
	{
		t = visi_test_bvh_fast(bvh, bvh->childs[idx] + res[i], shoot);
		if (t > 0)
			return (t);
	}
	return (0);
}

float	shadow_test_bvh(t_shoot *shoot, t_bvh *bvh, int idx, float dist_light)
{
	__m256		v_res;
	float		res[8];
	int			i;
	float		t;

	if (bvh->childs[idx] == -1)
		return (shadow_test_leafs(shoot, bvh->group[idx], dist_light, bvh->group_size[idx]));
	v_res = aabb_test_simd(bvh, bvh->childs[idx], shoot->shadow_ray, shoot->hit_pt);
	_mm256_storeu_ps(res, v_res);
	i = -1;
	while (++i < 8)
	{
		if (res[i] != -1)
		{
			t = shadow_test_bvh(shoot, bvh, bvh->childs[idx] + i, dist_light);
			if (t > 0)
				return (t);
		}
	}
	return (0);
}
