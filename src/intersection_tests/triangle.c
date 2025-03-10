#include "minirt.h"

int		all_same_sign(float bary[3])
{
	return ((bary[0] < 0 && bary[1] < 0 && bary[2] < 0)
			|| (bary[0] > 0 && bary[1] > 0 && bary[2] > 0));
}

// float	intersection_test_triangle_visi(t_object *obj, t_shoot *shoot, float t_min)
// {
// 	// add test for parrallelity !!! ... 
// 	float   px[3][3];
// 	float   bary[3];
// 	float   denom;
// 	int     i;
// 	float	tmp[3];
// 	float	tmp2[3];
// 	float	t;

// 	vec_substr(obj->geo.tri.v0, shoot->src, px[0]);
// 	vec_substr(obj->geo.tri.v1, shoot->src, px[1]);
// 	vec_substr(obj->geo.tri.v2, shoot->src, px[2]);
// 	bary[0] = triple_scalar(shoot->dir, px[2], px[1]);
// 	bary[1] = triple_scalar(shoot->dir, px[0], px[2]);
// 	bary[2] = triple_scalar(shoot->dir, px[1], px[0]);
// 	if (!all_same_sign(bary))
// 		return (0);
// 	denom = 1.0 / (bary[0] + bary[1] + bary[2]);
// 	i = -1;
// 	while (++i < 3)
// 		bary[i] *= denom;
// 	i = -1;
// 	while (++i < 3)
// 		tmp[i] = bary[0] * obj->geo.tri.v0[i] + bary[1] * obj->geo.tri.v1[i] + bary[2] * obj->geo.tri.v2[i];
// 	vec_substr(tmp, shoot->src, tmp2);
// 	t = dot_13_13(tmp2, shoot->dir);
// 	if (t > 0 && t < t_min)
// 		ft_memcpy(&shoot->bary, bary, sizeof(float[3])); // save the baycenter fo later (normal interpolation)
// 	return (t);
// }

float	intersection_test_triangle(t_object *obj, float ray[3], float origin[3])
{
	float   px[3][3];
	float   bary[3];
	float   denom;
	int     i;
	float	tmp[3];
	float	tmp2[3];

	vec_substr(obj->geo.tri.v0, origin, px[0]);
	vec_substr(obj->geo.tri.v1, origin, px[1]);
	vec_substr(obj->geo.tri.v2, origin, px[2]);
	bary[0] = triple_scalar(ray, px[2], px[1]);
	bary[1] = triple_scalar(ray, px[0], px[2]);
	bary[2] = triple_scalar(ray, px[1], px[0]);
	if (!all_same_sign(bary) || (bary[0] + bary[1] + bary[2]) == 0) // check that !!! add to visi
		return (0);
	denom = 1.0 / (bary[0] + bary[1] + bary[2]);
	i = -1;
	while (++i < 3)
		bary[i] *= denom;
	i = -1;
	while (++i < 3)
		tmp[i] = bary[0] * obj->geo.tri.v0[i] + bary[1] * obj->geo.tri.v1[i] + bary[2] * obj->geo.tri.v2[i];
	vec_substr(tmp, origin, tmp2);
	float t = dot_13_13(tmp2, ray);
	if (t > EPSILON)
		return (t);
	return (0);
}
