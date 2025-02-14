#include "minirt.h"

float	intersection_test_bvh_root(t_aabb *node, t_shoot *shoot);
float	intersection_test_bvh(t_aabb *node, t_shoot *shoot);
float	intersection_test_sphere(t_sphere *sphere, float p_ray[3], float origin[3]);
float	intersection_test_plane(t_plane *plane, float p_ray[3], float origin[3]);
float	intersection_test_aabb(t_aabb *aabb, float dir[3], float src[3]);
int		shadow_intersection_tests(t_shoot *shoot, t_object *objects, float shadow_ray[3], float dist_light, int n_obj);
float	shadow_test_bvh_root(t_shoot *shoot, t_aabb *node, float shadow_ray[3], float dist_light, int n_obj);
float	shadow_test_bvh(t_shoot *shoot, t_aabb *node, float shadow_ray[3], float dist_light, int n_obj);

float visibility_intersection_tests(t_object *objects, t_shoot *shoot, int n_obj)
{
	float		t_min;
	float		t;
	int			i;
	
	t_min = FLT_MAX;
	t = 0;
	atomic_fetch_add(&num_object_tests, 1); // perf logs
	i = -1;
	while (++i < n_obj)
	{
		if (objects[i].type == BVH)
			t = intersection_test_bvh_root(objects[i].geo, shoot);
		else if (objects[i].type == SPHERE)
			t = intersection_test_sphere(objects[i].geo, shoot->dir, shoot->src);
		else if (objects[i].type == PLANE)
		 	t = intersection_test_plane(objects[i].geo, shoot->dir, shoot->src);
		if (t > EPSILON && t < t_min)
		{
			t_min = t;
			if (objects[i].type != BVH)
				shoot->obj = objects + i;
		}
	}
	return (t_min);
}

float	intersection_test_bvh_root(t_aabb *node, t_shoot *shoot)
{
	float	t;

	t = intersection_test_aabb(node, shoot->dir, shoot->src);
	if (t < EPSILON)
		return (0);
	return (intersection_test_bvh(node, shoot));
}

float	intersection_test_bvh(t_aabb *node, t_shoot *shoot)
{
	float	t;
	float	t_left;
	float	t_right;

	if (!node->childs)
		return (visibility_intersection_tests(node->group, shoot, node->group_size));
	t_left 	= intersection_test_aabb(&node->childs[0], shoot->dir, shoot->src);
	t_right = intersection_test_aabb(&node->childs[1], shoot->dir, shoot->src);
	if (t_left < t_right + EPSILON)
	{
		t = intersection_test_bvh(&node->childs[0], shoot);
		if (t > EPSILON)
			return (t);
		return (intersection_test_bvh(&node->childs[1], shoot));
	}
	else
	{
		t = intersection_test_bvh(&node->childs[1], shoot);
		if (t > EPSILON)
			return (t);
		return (intersection_test_bvh(&node->childs[0], shoot));
	}
}

// duplicate function for shadow test for optimization: if discr >= 0 return 1
float	intersection_test_sphere(t_sphere *sphere, float p_ray[3], float origin[3])
{
	float	c_minus_o[3];
	float	a;
	float	b;
	float	c;
	float	discriminant;
	float	res[2];

	atomic_fetch_add(&num_object_intersections, 1); // perf logs
	
	c_minus_o[0] = sphere->center[0] - origin[0];
	c_minus_o[1] = sphere->center[1] - origin[1];
	c_minus_o[2] = sphere->center[2] - origin[2];
	a = dot_13_13(p_ray, p_ray);
	b = -2 * dot_13_13(p_ray, c_minus_o);
	c = dot_13_13(c_minus_o, c_minus_o) - sphere->radius * sphere->radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	discriminant = sqrtf(discriminant);
	res[0] = (-b - discriminant) / (2 * a);
	res[1] = (-b + discriminant) / (2 * a);
	if (res[0] > 0)
		return(res[0]);
	if (res[1] > 0)
		return(res[1]);
	return (0);
}

float	intersection_test_plane(t_plane *plane, float p_ray[3], float origin[3])
{
	float	a;
	float	pts_d[3];
	float	b;

	atomic_fetch_add(&num_object_intersections, 1); // perf logs

	a = dot_13_13(plane->normal, p_ray);
	if (a < EPSILON && a > -EPSILON)
		return (0);
	pts_d[0] = plane->point[0] - origin[0];
	pts_d[1] = plane->point[1] - origin[1];
	pts_d[2] = plane->point[2] - origin[2];
	b = dot_13_13(plane->normal, pts_d);
	if ((a < 0 && b > 0) || (a > 0 && b < 0)) // performs better than if (b > 0) after (b / a)
		return (0);
	return (b / a);
}

float	intersection_test_aabb(t_aabb *aabb, float dir[3], float src[3])
{
	return (1);
}

/* returns 0 as soon as one object is in the way of light, returns 1 when no shadow*/
int shadow_intersection_tests(t_shoot *shoot, t_object *objects, float shadow_ray[3], float dist_light, int n_obj)
{
	float 		t;
	int			i;

	i = 0;
	while (i < n_obj)
	{
		if (objects[i].type == BVH)
			t = shadow_test_bvh_root(shoot, objects[i].geo, shadow_ray, dist_light, n_obj);
		else if (objects[i].type == SPHERE)
			t = intersection_test_sphere(objects[i].geo, shadow_ray, shoot->hit_pt);
		else if (objects[i].type == PLANE)
		 	t = intersection_test_plane(objects[i].geo, shadow_ray, shoot->hit_pt);
		if (t > EPSILON && dist_light > t)
			return (1);
		i++;
	}
	return (0);
}

float	shadow_test_bvh_root(t_shoot *shoot, t_aabb *node, float shadow_ray[3], float dist_light, int n_obj)
{
	float	t;

	t = intersection_test_aabb(node, shadow_ray, shoot->hit_pt);
	if (t < EPSILON)
		return (0);
	return (shadow_test_bvh(shoot, node, shadow_ray, dist_light, n_obj));
}


float	shadow_test_bvh(t_shoot *shoot, t_aabb *node, float shadow_ray[3], float dist_light, int n_obj)
{
	float	t;

	if (!node->childs)
		return (shadow_intersection_tests(shoot, node->group, shadow_ray, dist_light, node->group_size));
	if (intersection_test_aabb(&node->childs[0], shadow_ray, shoot->hit_pt) > EPSILON)
	{
		t = shadow_test_bvh(shoot, &node->childs[0], shadow_ray, dist_light, n_obj);
		if (t > EPSILON)
			return (t);
	}
	if (intersection_test_aabb(&node->childs[1], shadow_ray, shoot->hit_pt) > EPSILON)
	{
		t = shadow_test_bvh(shoot, &node->childs[1], shadow_ray, dist_light, n_obj);
		if (t > EPSILON)
			return (t);
	}
	return (0);
}
