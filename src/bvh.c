#include "minirt.h"

void	cut_in_right_left_nodes(t_aabb *node);

t_aabb	*init_bvh(t_object *objects)
{
	t_aabb		*root;
	t_object	*obj;
	int 		i;
	int 		j;
	float		min;
	float		max;
	float		tmp_min;
	float		tmp_max;

	root = malloc(sizeof(t_aabb));

	// get goup_size
	obj = objects;
	root->group_size = 0;
	while (obj->type != END)
	{
		if (obj->type != PLANE)
			root->group_size++;
		obj++;
	}

	// copy list without plane
	root->group = malloc(sizeof(t_object) * root->group_size);
	obj = objects;
	i = 0;
	while (obj->type != END)
	{
		if (obj->type != PLANE)
		{
			root->group[i] = *obj;
			i++;
		}
		obj++;
	}
	
	// get pt_min / pt_max
	i = -1;
	while (++i < 3)
	{
		min = FLT_MAX;
		max = FLT_MIN;
		j = -1;
		while (++j < root->group_size)
		{
			// switch sur les differents primitives. 
			// For the moment only sphere
			tmp_min = ((t_sphere *)(root->group[j].geo))->center[i] - ((t_sphere *)(root->group[j].geo))->radius ; 
			if (tmp_min < min)
				min = tmp_min;

			tmp_max = ((t_sphere *)(root->group[j].geo))->center[i] + ((t_sphere *)(root->group[j].geo))->radius ;
			if (tmp_max > max)
				max = tmp_max; 
		}
		root->pt_min[i] = min;
		root->pt_max[i] = max;
	}
	// cut in right and left.
	cut_in_right_left_nodes(root);
	return (root);
}

void	cut_in_right_left_nodes(t_aabb *node)
{
	float		max;
	float		tmp;
	int			axis;
	int 		i;
	int 		i_left;
	int 		i_right;
	float		mid;
	float 		center;
	
	// exit condition :
	if (node->group_size <= MAX_BVH_GROUP)
	{
		node->childs = NULL;
		return ;
	}
	// find the largest axis
	i = -1;
	max = FLT_MIN;
	while (++i < 3) // MACROS?
	{
		tmp = node->pt_max[i] - node->pt_min[i];
		if (tmp > max)
		{
			max = tmp;
			axis = i;
		}
	}

	// mid = middle of largest axis
	mid = (node->pt_max[axis] + node->pt_min[axis]) / 2;

	// create left and right
	node->childs = malloc(sizeof(t_aabb) * 2);

	// calculate geo for left and right;
	cpy_vec(node->childs[0].pt_min, node->pt_min);
	cpy_vec(node->childs[0].pt_max, node->pt_max);
	cpy_vec(node->childs[1].pt_min, node->pt_min);
	cpy_vec(node->childs[1].pt_max, node->pt_max);

	node->childs[0].pt_max[axis] = mid;
	node->childs[1].pt_min[axis] = mid;

 	// partion in left and right ( first-> size / malloc group / fill group)
	i = 0;
	node->childs[0].group_size = 0;
	node->childs[1].group_size = 0;
	while (i < node->group_size) 
	{
		center = ((t_sphere *)node->group[i].geo)->center[axis]; // need for switch when cylinders
		if (center < mid)			
			node->childs[0].group_size++;
		else
			node->childs[1].group_size++;
		i++;
	}
	node->childs[0].group = malloc(sizeof(t_object) * node->childs[0].group_size);
	node->childs[1].group = malloc(sizeof(t_object) * node->childs[1].group_size);
	i = -1;
	i_left = 0;
	i_right = 0;
	while (++i < node->group_size) 
	{
		center = ((t_sphere *)node->group[i].geo)->center[axis]; // need for switch when cylinders
		if (center < mid)			
			node->childs[0].group[i_left++] = node->group[i];
		else
			node->childs[1].group[i_right++] = node->group[i];
	}
	cut_in_right_left_nodes(&node->childs[0]);
	cut_in_right_left_nodes(&node->childs[1]);
}
