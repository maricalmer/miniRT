#include "minirt.h"

void	cut_in_right_left_nodes(t_aabb *node);
void	get_bbox_min_max(t_aabb *node);

t_aabb	*init_bvh(t_data *data)
{
	t_aabb		*root;
	t_object	*obj;
	int 		i;
	int			j;

	root = malloc(sizeof(t_aabb));

	root->depth = 0;
	// get goup_size
	obj = data->objects;
	root->group_size = 0;
	i = -1;
	while (++i < data->n_obj)
	{
		if (obj->type == SPHERE)
			root->group_size++;
		obj++;
	}

	// copy list without plane
	root->group = malloc(sizeof(t_object) * root->group_size);
	obj = data->objects;
	i = -1;
	j = 0;
	while (++i < data->n_obj)
	{
		if (obj[i].type == SPHERE)
			root->group[j++] = obj[i];
	}
	// cut in right and left.
	cut_in_right_left_nodes(root);
	return (root);
}

void	get_bbox_min_max(t_aabb *node)
{
	// get pt_min / pt_max
	int		i;
	int		j;
	float	min;
	float	max;
	float	tmp_min;
	float	tmp_max;

	i = -1;
	while (++i < 3)
	{
		min = FLT_MAX;
		max = -FLT_MAX;
		j = -1;
		while (++j < node->group_size)
		{
			// switch sur les differents primitives. 
			// For the moment only sphere
			tmp_min = ((t_sphere *)(node->group[j].geo))->center[i] - ((t_sphere *)(node->group[j].geo))->radius ; 
			if (tmp_min < min)
				min = tmp_min;

			tmp_max = ((t_sphere *)(node->group[j].geo))->center[i] + ((t_sphere *)(node->group[j].geo))->radius ;
			if (tmp_max > max)
				max = tmp_max; 
		}
		node->pt_min[i] = min;
		node->pt_max[i] = max;
	}
}

void	cut_in_right_left_nodes(t_aabb *node)
{
	// handle the case if child box empty ? cut at the median or average ??

	float		max;
	float		tmp;
	int			axis;
	int 		i;
	int 		i_left;
	int 		i_right;
	float		mid;
	float 		center;
	
	get_bbox_min_max(node);
	// exit condition :
	if (node->group_size <= MAX_BVH_GROUP || node->depth == BVH_DEPTH_MAX)
	{
		node->childs = NULL;
		return ;
	}
	// find the largest axis OR FIND the axis with the max effective cut ??!!! .... OR EXIT at depth > LIMIT...
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

	// FIND THE cutting plane :)
	// mid = middle of largest axis
		// mid = (node->pt_max[axis] + node->pt_min[axis]) / 2;
	// OR BETTER mid = average of centroids
		// mid = 0;
		// i = -1;
		// while (++i < node->group_size)
		// 	mid += ((t_sphere *)node->group[i].geo)->center[axis];
		// mid /= node->group_size;
	// EVEN BETTER -> la mediane !
	float centers[node->group_size];
	i = -1;
	while (++i < node->group_size)
		centers[i] = ((t_sphere *)node->group[i].geo)->center[axis];
	mid = findMedian(centers, node->group_size);



	// create left and right
	node->childs = malloc(sizeof(t_aabb) * 2);

	// partion in left and right ( first-> size / malloc group / fill group)
	i = 0;
	node->childs[0].group_size = 0;
	node->childs[1].group_size = 0;
	while (i < node->group_size) 
	{
		center = ((t_sphere *)node->group[i].geo)->center[axis]; // need for switch when cylinders
		if (center <= mid + ((t_sphere *)node->group[i].geo)->radius)			
			node->childs[0].group_size++;
		if (center >  mid - ((t_sphere *)node->group[i].geo)->radius)
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
		if (center <= mid + ((t_sphere *)node->group[i].geo)->radius)			
			node->childs[0].group[i_left++] = node->group[i];
		if (center >  mid - ((t_sphere *)node->group[i].geo)->radius)
			node->childs[1].group[i_right++] = node->group[i];
	}
	// if (node->depth == BVH_DEPTH_MAX - 1)
	// {
	// 	printf("left : %i\n", node->childs[0].group_size);
	// 	printf("right : %i\n\n", node->childs[1].group_size);
	// }
	node->childs[0].depth = node->depth + 1;
	node->childs[1].depth = node->depth + 1;

	if (node->childs[0].group_size == node->group_size)
		node->childs[0].depth = BVH_DEPTH_MAX;
	if (node->childs[1].group_size == node->group_size)
		node->childs[1].depth = BVH_DEPTH_MAX;
	// or even better ==> cut on the other second largest axis !!!	

	cut_in_right_left_nodes(&node->childs[0]);
	cut_in_right_left_nodes(&node->childs[1]);
	free(node->group);
}

void	update_group(t_data *data, t_aabb *root)
{
	t_object	*new_objects;
	int			plane_counter;
	int			i;
	int			j;

	i = 0;
	plane_counter = 0;
	while (i < data->n_obj)
	{
		if (data->objects[i].type != SPHERE)
			plane_counter++;
		i++;
	}
	new_objects = malloc(sizeof(t_object) * (plane_counter + 1));
	new_objects[0].type = BVH;
	new_objects[0].geo = (void *)root;
	i = 0;
	j = 1;
	while (i < data->n_obj)
	{
		if (data->objects[i].type != SPHERE)
			ft_memcpy(&new_objects[j++], &data->objects[i], sizeof(t_object));
		i++;
	}
	free(data->objects);
	data->objects = new_objects;
	data->n_obj = plane_counter + 1;
}
