#include "minirt.h"

/* returns t>epsilon as soon as one object is in the way of light, returns 0 when hit = shadow*/
float shadow_intersection_tests(t_shoot *shoot, t_object *objects, float dist_light, int n_obj)
{
	float 		t;
	int			i;

	t = 0;
	i = 0;
	while (i < n_obj)
	{
        if (objects[i].type == SPHERE)
			t = intersection_test_sphere(&objects[i], shoot->shadow_ray, shoot->hit_pt);
		else if (objects[i].type == PLANE)
		 	t = intersection_test_plane(&objects[i], shoot->shadow_ray, shoot->hit_pt);
		else if (objects[i].type == TRI)
		 	t = intersection_test_triangle(&objects[i], shoot->shadow_ray, shoot->hit_pt);
		else if (objects[i].type == CYLINDER)
		 	t = intersection_test_cylinder(&objects[i], shoot->shadow_ray, shoot->hit_pt);
		else if (objects[i].type == BVH)
			t = shadow_test_bvh(shoot, objects[i].geo.bvh, 0, dist_light);
		if (t > EPSILON && dist_light > t)
			return (t);
		i++;
	}
	return (0);
}

float shadow_intersection_tests_leaf(t_shoot *shoot, t_object **objects, float dist_light, int n_obj)
{
	float 		t;
	int			i;

	t = 0;
	i = 0;
	while (i < n_obj)
	{
        if (objects[i]->type == SPHERE)
			t = intersection_test_sphere(objects[i], shoot->shadow_ray, shoot->hit_pt);
		else if (objects[i]->type == TRI)
			t = intersection_test_triangle(objects[i], shoot->shadow_ray, shoot->hit_pt);
		else if (objects[i]->type == CYLINDER)
		 	t = intersection_test_cylinder(objects[i], shoot->shadow_ray, shoot->hit_pt);
		if (t > EPSILON && dist_light > t)
			return (t);
		i++;
	}
	return (0);
}
