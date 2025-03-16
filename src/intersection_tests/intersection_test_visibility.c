#include "minirt.h"

float visibility_intersection_tests(t_object *objects, t_shoot *shoot, int n_obj)
{
	float		t_min;
	float		t;
	int			i;
	
	t_min = FLT_MAX;
	t = 0;
	i = -1;
	while (++i < n_obj)
	{
		if (objects[i].type == SPHERE)
			t = intersection_test_sphere(&objects[i], shoot->dir, shoot->src);
		else if (objects[i].type == PLANE)
		 	t = intersection_test_plane(&objects[i], shoot->dir, shoot->src);
		else if (objects[i].type == TRI)
			 t = intersection_test_triangle(&objects[i], shoot->dir, shoot->src);
		else if (objects[i].type == CYLINDER)
		 	t = intersection_test_cylinder(&objects[i], shoot->dir, shoot->src);
		else if (objects[i].type == BVH)
			t = intersection_test_bvh(objects[i].geo.bvh, 0, shoot);
		if (t > EPSILON && t < t_min)
		{
			t_min = t;
			if (objects[i].type != BVH)
				shoot->obj = objects + i;
		}
	}
	if (t_min != FLT_MAX)
		return (t_min);
	return (0);
}


float visibility_intersection_tests_leafs(t_object **objects, t_shoot *shoot, int n_obj)
{
	float		t_min;
	float		t;
	int			i;
	
	t_min = FLT_MAX;
	t = 0;
	i = -1;
	while (++i < n_obj)
	{
		if (objects[i]->type == SPHERE)
			t = intersection_test_sphere(objects[i], shoot->dir, shoot->src);
		else if (objects[i]->type == TRI)
			t = intersection_test_triangle(objects[i], shoot->dir, shoot->src);
		else if (objects[i]->type == CYLINDER)
		 	t = intersection_test_cylinder(objects[i], shoot->dir, shoot->src);
		if (t > EPSILON && t < t_min)
		{
			t_min = t;
			shoot->obj = objects[i];
		}
	}
	if (t_min != FLT_MAX)
		return (t_min);
	return (0);
}
