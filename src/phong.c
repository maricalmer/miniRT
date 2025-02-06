#include "minirt.h"

int shadow_intersection_tests(t_intersect *first, t_data *data, float shadow_ray[3]);

int	phong(t_intersect *first, t_data *data, float p_ray[3])
{
	unsigned char	*rgb;
	int				res[3];
	int				i;
	float			theta_LN;
	float			shadow_ray[3];
	float			reflection_ray[3];
	float			R_dot_E;

	if (!first->obj)
		return (0);
	// get first the rgb.
	if (first->obj->type == SPHERE)
		rgb = ((t_sphere *)(first->obj->data))->rgb;
	else if (first->obj->type == PLANE)
		rgb = ((t_plane *)(first->obj->data))->rgb;
	// other SWITCHES ....
	// ambiant part ==> at the moment white light !
	 i = -1;
	 while (++i < 3)
		res[i] = data->ambient.brightness * rgb[i];
	// lights ==> at the moment white light and only one !
	shadow_ray[0] = data->lights[0].origin[0] - first->hit_pt[0];
	shadow_ray[1] = data->lights[0].origin[1] - first->hit_pt[1];
	shadow_ray[2] = data->lights[0].origin[2] - first->hit_pt[2];
	normalize(shadow_ray);
	theta_LN = dot_13_13(first->normal, shadow_ray);
	reflection_ray[0] = -shadow_ray[0] + 2 * theta_LN * first->normal[0];
	reflection_ray[1] = -shadow_ray[1] + 2 * theta_LN * first->normal[1];
	reflection_ray[2] = -shadow_ray[2] + 2 * theta_LN * first->normal[2];
	R_dot_E = - dot_13_13(reflection_ray, p_ray);
	R_dot_E = fmaxf(0, R_dot_E);
	R_dot_E = powf(R_dot_E, SPECULAR_POWER);
	if (theta_LN > 0 && shadow_intersection_tests(first, data, shadow_ray))
	{
		i = -1;
	 	while (++i < 3)
		{
			res[i] += data->lights[0].brightness * theta_LN * rgb[i];
			res[i] += data->lights[0].brightness * R_dot_E * rgb[i];
		}
	}
	


	i = -1;
	while (++i < 3)
	{
		if (res[i] > 255)
			res[i] = 255;
	}
	return (res[0] << 16 | res[1] << 8 | res[2]);
}
/* returns 0 as soon as one object is in the way of light, returns 1 when no shadow*/
int shadow_intersection_tests(t_intersect *first, t_data *data, float shadow_ray[3])
{
	t_object	*obj;
	float 		t;

	obj = data->objects;
	while (obj->type)
	{
		if (obj->type == SPHERE)
			t = intersection_test_sphere(obj->data, shadow_ray, first->hit_pt); // world_space , return 0 if no intersection
		else if (obj->type == PLANE)
		 	t = intersection_test_plane(obj->data, shadow_ray, first->hit_pt); // world_space , return 0 if no intersection
		if (t > EPSILON)
			return (0);
		obj++;
	}
	return (1);
}
