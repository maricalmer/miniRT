#include "minirt.h"

float	intersection_test_sphere(t_object *obj, float ray[3], float origin[3])
{
	float	c_minus_o[3];
	float	a;
	float	b;
	float	c;
	float	discriminant;
	float	res[2];

	vec_substr(obj->geo.sph.center, origin, c_minus_o);
	a = dot_13_13(ray, ray);
	b = -2 * dot_13_13(ray, c_minus_o);
	c = dot_13_13(c_minus_o, c_minus_o) - obj->geo.sph.radius * obj->geo.sph.radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	discriminant = sqrtf(discriminant);
	res[0] = (-b - discriminant) / (2 * a);
	res[1] = (-b + discriminant) / (2 * a);
	if (res[0] > EPSILON)
		return(res[0]);
	if (res[1] > EPSILON)
		return(res[1]);
	return (0);
}

/* for returning the second hit_pt*/
float	intersection_test_sphere2(t_object	*obj, float ray[3], float origin[3])
{
	float	c_minus_o[3];
	float	a;
	float	b;
	float	c;
	float	discriminant;

	vec_substr(obj->geo.sph.center, origin, c_minus_o);
	a = dot_13_13(ray, ray);
	b = -2 * dot_13_13(ray, c_minus_o);
	c = dot_13_13(c_minus_o, c_minus_o) - obj->geo.sph.radius * obj->geo.sph.radius;
	discriminant = b * b - 4 * a * c;
	discriminant = sqrtf(discriminant);
	return ((-b + discriminant) / (2 * a));
}