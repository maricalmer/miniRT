#include "minirt.h"

float	intersection_test_cylinder(t_object *obj, float ray[3], float origin[3])
{
	float   r_ao[3];
    float   v_a[3];
    float   tmp[3];
    float   tmp2[3];
    float	a;
	float	b;
	float	c;
	float	discriminant;
	float	res[2];
	
	vec_substr(origin, obj->geo.cyl.center, tmp);
    cprod_13_13(tmp, obj->geo.cyl.dir, tmp2);
    cprod_13_13(obj->geo.cyl.dir, tmp2, r_ao);

    cprod_13_13(ray, obj->geo.cyl.dir, tmp);
    cprod_13_13(obj->geo.cyl.dir, tmp, v_a);

    a = dot_13_13(v_a, v_a);
    b = 2 * dot_13_13(r_ao, v_a);
	c = dot_13_13 (r_ao, r_ao) - obj->geo.cyl.radius * obj->geo.cyl.radius;

	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	discriminant = sqrtf(discriminant);
	res[0] = (-b - discriminant) / (2 * a);
	res[1] = (-b + discriminant) / (2 * a);

	if (res[0] < EPSILON)
		res[0] = res[1];

	// check of inside the limits ...
	// first for the first hit_pt
	
	int i = -1;
	while (++i < 3)
		tmp[i] = origin[i] + res[0] * ray[i] - obj->geo.cyl.center[i];
	float	h = dot_13_13(tmp, obj->geo.cyl.dir);
	if (h >= - obj->geo.cyl.height / 2 && h <= obj->geo.cyl.height / 2)
		return (res[0]);
	
	// if not, then we check the second hit pt.
	i = -1;
	while (++i < 3)
		tmp[i] = origin[i] + res[1] * ray[i] - obj->geo.cyl.center[i];
	h = dot_13_13(tmp, obj->geo.cyl.dir);
	if (h >= - obj->geo.cyl.height / 2 && h <= obj->geo.cyl.height / 2)
		return (res[1]);
	
	// if completly outside limits : 
	return (0);
}
