#include "minirt.h"

int		all_same_sign(float bary[3])
{
	return ((bary[0] < 0 && bary[1] < 0 && bary[2] < 0)
			|| (bary[0] > 0 && bary[1] > 0 && bary[2] > 0));
}
/* Möller-Trumbore Ray-Triangle Intersection Test */
float test_triangle(t_object *obj, float ray[3], float origin[3]) 
{
    float edge1[3], edge2[3], h[3], s[3], q[3];
    float det, inv_det, u, v, t;

    // Compute edges of the triangle
    vec_substr(obj->geo.tri.v1, obj->geo.tri.v0, edge1);
   	vec_substr(obj->geo.tri.v2, obj->geo.tri.v0, edge2);

    // Compute the determinant
    cprod_13_13(ray, edge2, h);
    det = dot_13_13(edge1, h);

    // If the determinant is near zero, the ray is parallel to the triangle
    if (fabs(det) < EPSILON)
        return 0.0f;

    inv_det = 1.0f / det;
    vec_substr(origin, obj->geo.tri.v0, s);
    u = dot_13_13(s, h) * inv_det;

    // Check if u is outside the triangle
    if (u < 0.0f || u > 1.0f)
        return 0.0f;

    cprod_13_13(s, edge1, q);
    v = dot_13_13(ray, q) * inv_det;

    // Check if v is outside the triangle
    if (v < 0.0f || u + v > 1.0f)
        return 0.0f;

    // Compute t (distance from ray origin to intersection point)
    t = dot_13_13(edge2, q) * inv_det;

    return (t); // Return t if the intersection is in front of the ray
}

/*

float	test_triangle_LEGACY(t_object *obj, float ray[3], float origin[3])
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
	if (!all_same_sign(bary) || (bary[0] + bary[1] + bary[2]) == 0)
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
*/
