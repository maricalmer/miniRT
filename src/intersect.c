#include "minirt.h"

float	intersection_test_bvh_root(t_aabb *node, t_shoot *shoot);
float	intersection_test_bvh(t_aabb *node, t_shoot *shoot);
float	intersection_test_sphere(t_sphere *sphere, float p_ray[3], float origin[3]);
float	intersection_test_plane(t_plane *plane, float p_ray[3], float origin[3]);
float	intersection_test_aabb(t_aabb *aabb, float dir[3], float src[3]);
float	shadow_intersection_tests(t_shoot *shoot, t_object *objects, float shadow_ray[3], float dist_light, int n_obj);
float	shadow_test_bvh_root(t_shoot *shoot, t_aabb *node, float shadow_ray[3], float dist_light);
float	shadow_test_bvh(t_shoot *shoot, t_aabb *node, float shadow_ray[3], float dist_light);

float visibility_intersection_tests(t_object *objects, t_shoot *shoot, int n_obj)
{
	float		t_min;
	float		t;
	int			i;
	
	t_min = FLT_MAX;
	t = 0;
	//atomic_fetch_add(&num_object_tests, 1); // perf logs
	i = -1;
	while (++i < n_obj)
	{
		if (objects[i].type == SPHERE)
			t = intersection_test_sphere(objects[i].geo, shoot->dir, shoot->src);
		else if (objects[i].type == PLANE)
		 	t = intersection_test_plane(objects[i].geo, shoot->dir, shoot->src);
		else if (objects[i].type == BVH)
			t = intersection_test_bvh_root(objects[i].geo, shoot);
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

float	intersection_test_bvh_root(t_aabb *node, t_shoot *shoot)
{
	float	t;

	t = intersection_test_aabb(node, shoot->dir, shoot->src);
	if (t == -1)
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
	if (t_left == -1 && t_right == -1)
		return (0);
	else if (t_left != -1 && t_right != -1)
	{
		if (t_left < t_right) // what if equal ???!!!
		{
			t = intersection_test_bvh(&node->childs[0], shoot);
			if (t > 0)
				return (t);
			return (intersection_test_bvh(&node->childs[1], shoot));
		}
		else
		{
			t = intersection_test_bvh(&node->childs[1], shoot);
			if (t > 0)
				return (t);
			return (intersection_test_bvh(&node->childs[0], shoot));
		}
	}
	else if (t_right == -1)
		return (intersection_test_bvh(&node->childs[0], shoot));
	else if (t_left == -1)
		return (intersection_test_bvh(&node->childs[1], shoot));
	return (100);
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

	// atomic_fetch_add(&num_object_intersections, 1); // perf logs
	
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

float	intersection_test_sphere2(t_sphere *sphere, float p_ray[3], float origin[3])
{
	float	c_minus_o[3];
	float	a;
	float	b;
	float	c;
	float	discriminant;

	c_minus_o[0] = sphere->center[0] - origin[0];
	c_minus_o[1] = sphere->center[1] - origin[1];
	c_minus_o[2] = sphere->center[2] - origin[2];
	a = dot_13_13(p_ray, p_ray);
	b = -2 * dot_13_13(p_ray, c_minus_o);
	c = dot_13_13(c_minus_o, c_minus_o) - sphere->radius * sphere->radius;
	discriminant = b * b - 4 * a * c;
	discriminant = sqrtf(discriminant);
	return ((-b + discriminant) / (2 * a));
}

float	intersection_test_plane(t_plane *plane, float p_ray[3], float origin[3])
{
	float	a;
	float	pts_d[3];
	float	b;

	// atomic_fetch_add(&num_object_intersections, 1); // perf logs

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
	float	t_min;
	float	t_max;
	float	ood;
	float	t1;
	float	t2;
	int		i;

	t_min = 0;
	t_max = FLT_MAX;
	i = -1;
	while (++i < 3)
	{
		if (fabsf(dir[i]) < EPSILON)
		{
			if (src[i] < aabb->pt_min[i] || src[i] > aabb->pt_max[i])
				return (0);
		}
		else
		{
			ood = 1.0 / dir[i];
			t1 = (aabb->pt_min[i] - src[i]) * ood;
			t2 = (aabb->pt_max[i] - src[i]) * ood;
			if (t1 > t2)
				ft_swap(&t1, &t2);
			t_min = fmaxf(t_min, t1);
			t_max = fminf(t_max, t2);
			if (t_min > t_max)
				return (-1);
		}
	}
	return (t_min);
}

/* returns t>epsilon as soon as one object is in the way of light, returns 0 when hit = shadow*/
float shadow_intersection_tests(t_shoot *shoot, t_object *objects, float shadow_ray[3], float dist_light, int n_obj)
{
	float 		t;
	int			i;

	t = 0;
	i = 0;
	while (i < n_obj)
	{
		if (objects[i].type == SPHERE)
			t = intersection_test_sphere(objects[i].geo, shadow_ray, shoot->hit_pt);
		else if (objects[i].type == PLANE)
		 	t = intersection_test_plane(objects[i].geo, shadow_ray, shoot->hit_pt);
		else if (objects[i].type == BVH)
			t = shadow_test_bvh_root(shoot, objects[i].geo, shadow_ray, dist_light);
		if (t > EPSILON && dist_light > t)
			return (t);
		i++;
	}
	return (0);
}

float	shadow_test_bvh_root(t_shoot *shoot, t_aabb *node, float shadow_ray[3], float dist_light)
{
	float	t;

	t = intersection_test_aabb(node, shadow_ray, shoot->hit_pt);
	if (t == -1)
		return (0);
	return (shadow_test_bvh(shoot, node, shadow_ray, dist_light));
}


float	shadow_test_bvh(t_shoot *shoot, t_aabb *node, float shadow_ray[3], float dist_light)
{
	float	t;
	float	t_left;
	float	t_right;

	if (!node->childs)
		return (shadow_intersection_tests(shoot, node->group, shadow_ray, dist_light, node->group_size));
	t_left 	= intersection_test_aabb(&node->childs[0], shadow_ray, shoot->hit_pt);
	t_right = intersection_test_aabb(&node->childs[1], shadow_ray, shoot->hit_pt);
	if (t_right == -1 && t_left == -1)
		return (0);
	if (t_right == -1)
		return (shadow_test_bvh(shoot, &node->childs[0], shadow_ray, dist_light));
	if (t_left == -1)
		return (shadow_test_bvh(shoot, &node->childs[1], shadow_ray, dist_light));
	if (t_left > t_right) // changer cette logique pour "box ou le ray parcouru est le plus long!..."
	{
		t = shadow_test_bvh(shoot, &node->childs[0], shadow_ray, dist_light);
		if (t != 0)
			return (t);
		return (shadow_test_bvh(shoot, &node->childs[1], shadow_ray, dist_light));
	}
	t = shadow_test_bvh(shoot, &node->childs[1], shadow_ray, dist_light);
	if (t != 0)
		return (t);
	return (shadow_test_bvh(shoot, &node->childs[0], shadow_ray, dist_light));
}


