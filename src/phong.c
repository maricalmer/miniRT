#include "minirt.h"

int shadow_intersection_tests(t_intersect *first, t_data *data, float shadow_ray[3]);

int	phong(t_intersect *first, t_data *data)
{
	unsigned char	*rgb;
	unsigned char	res[3];
	int				i;
	float			theta_LN;
	float			shadow_ray[3];
	
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
	if (theta_LN > 0 && shadow_intersection_tests(first, data, shadow_ray))
	{
		 i = -1;
	 	while (++i < 3)
			res[i] += data->lights[0].brightness * theta_LN * rgb[i]; // cap that thing to 255 ...
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
