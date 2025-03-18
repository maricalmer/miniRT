#include "minirt.h"

float visi_tests(t_object *objects, t_shoot *shoot, int n_obj)
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
			t = test_sphere(&objects[i], shoot->dir, shoot->src);
		else if (objects[i].type == PLANE)
		 	t = test_plane(&objects[i], shoot->dir, shoot->src);
		else if (objects[i].type == TRI)
			 t = test_triangle(&objects[i], shoot->dir, shoot->src);
		else if (objects[i].type == CYLINDER)
		 	t = test_cylinder(&objects[i], shoot->dir, shoot->src);
		else if (objects[i].type == BVH)
		{
			if (FAST_BVH_TRANSVERSAL)
				t = visi_test_bvh_fast(objects[i].geo.bvh, 0, shoot);
			else
				t = visi_test_bvh_strict(objects[i].geo.bvh, 0, shoot);
		}
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


float visi_test_leafs(t_object **objects, t_shoot *shoot, int n_obj)
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
			t = test_sphere(objects[i], shoot->dir, shoot->src);
		else if (objects[i]->type == TRI)
			t = test_triangle(objects[i], shoot->dir, shoot->src);
		else if (objects[i]->type == CYLINDER)
		 	t = test_cylinder(objects[i], shoot->dir, shoot->src);
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
