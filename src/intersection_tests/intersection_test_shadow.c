#include "minirt.h"

/* returns t>epsilon as soon as one object is in the way of light, returns 0 when hit = shadow*/
float shadow_intersection_tests(t_shoot *shoot, t_object *objects, float shadow_ray[3], float dist_light, int n_obj)
{
	float 		t;
	int			i;

	t = 0;
	i = 0;
	while (i < n_obj)
	{
		// if (i + 1 < n_obj)
        //     __builtin_prefetch(&objects[i + 1], 0, 1);  // 0 = read, 1 = temporal (prefetch for later use)
        if (objects[i].type == SPHERE)
			t = intersection_test_sphere(&objects[i], shadow_ray, shoot->hit_pt);
		else if (objects[i].type == PLANE)
		 	t = intersection_test_plane(&objects[i], shadow_ray, shoot->hit_pt);
		else if (objects[i].type == TRI)
		 	t = intersection_test_triangle(&objects[i], shadow_ray, shoot->hit_pt);
		// else if (objects[i].type == CYLINDER)
		//  	t = intersection_test_cylinder(objects[i].geo, shadow_ray, shoot->hit_pt);
		else if (objects[i].type == BVH)
			t = shadow_test_bvh(shoot, objects[i].geo.bvh, 0, shadow_ray, dist_light);
		if (t > EPSILON && dist_light > t)
			return (t);
		i++;
	}
	return (0);
}

float shadow_intersection_tests_leaf(t_shoot *shoot, t_object **objects, float shadow_ray[3], float dist_light, int n_obj)
{
	float 		t;
	int			i;

	t = 0;
	i = 0;
	while (i < n_obj)
	{
		// if (i + 1 < n_obj)
        //     __builtin_prefetch(&objects[i + 1], 0, 1);  // 0 = read, 1 = temporal (prefetch for later use)
        if (objects[i]->type == SPHERE)
			t = intersection_test_sphere(objects[i], shadow_ray, shoot->hit_pt);
		else if (objects[i]->type == TRI)
			t = intersection_test_triangle(objects[i], shadow_ray, shoot->hit_pt);
		// else if (objects[i].type == CYLINDER)
		//  	t = intersection_test_cylinder(objects[i].geo, shadow_ray, shoot->hit_pt);
		if (t > EPSILON && dist_light > t)
			return (t);
		i++;
	}
	return (0);
}