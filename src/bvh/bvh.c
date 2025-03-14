#include "minirt.h"

void	get_group_size(t_data *data, t_bvh *bvh);
void	copy_bvh_objects(t_data *data, t_bvh *bvh);
t_obj_geo *create_obj_geo_data(t_bvh *bvh);
t_obj_geo extract_geo_data_sphere(t_object *obj);
t_obj_geo extract_geo_data_tri(t_object *obj);
t_bvh   *init_bvh(t_data *data);
int		cut_into_child_nodes(t_bvh *bvh, int idx);
void 	get_child_sizes(t_bvh *bvh, int idx, float mid[3], int idx_left, int idx_right);
void	create_child_groups(t_bvh *bvh, int idx, int axis, float mid, int idx_left, int idx_right);
void	cut_in_two(t_bvh *bvh, int idx, int idx_c, int i);
int 	find_largest_axis(t_bvh *bvh, int idx);
float	find_cutting_plane(t_bvh *bvh, int idx, int axis);
void	get_bbox_min_max(t_bvh *bvh, int idx);
void 	get_bbox(t_bvh *bvh, int idx_child, int idx_parent, float mid, int axis, int side);
t_bbox	get_bbox_node(t_bvh *bvh, int idx, float mid, int axis, int side);
t_bbox	get_bbox_elem(t_bvh *bvh, int idx);
void	save_bbox_min(t_bvh *bvh, t_bbox bbox_node, t_bbox bbox_elem, int idx);

void	get_group_size(t_data *data, t_bvh *bvh)
{
	t_object	*obj;
	int			i;

	obj = data->objects;
	bvh->group_size[0] = 0;
	i = -1;
	while (++i < data->n_obj)
	{
		if (obj->type == SPHERE || obj->type == TRI)
			bvh->group_size[0]++;
		obj++;
	}
}

void	copy_bvh_objects(t_data *data, t_bvh *bvh)
{
	t_object	*obj;
	int			i;
	int			j;
	
	bvh->group[0] = malloc(sizeof(t_object *) * bvh->group_size[0]);
	obj = data->objects;
	i = -1;
	j = 0;
	while (++i < data->n_obj)
	{
		if (obj[i].type == SPHERE || obj[i].type == TRI)
			bvh->group[0][j++] = &obj[i];
	}
}

void print_nodes(t_bvh *bvh, int i)
{
	while ( i<200)//bvh->childs[i] != -1)
	{
		printf("\nidx : %i\n", i);
		printf("bbox : %.2f %.2f ;  %.2f %.2f ; %.2f %.2f\n", bvh->min_x[i], bvh->max_x[i], bvh->min_y[i], bvh->max_y[i], bvh->min_z[i], bvh->max_z[i]);
		int n_obj = bvh->group_size[i];
		printf("n_obj : %i\n", n_obj);
		// if (bvh->childs[i] == -1)
		// {
		// 	int j = -1;
		// 	while (++j < n_obj)
		// 	{
		// 		printf("\t%i\n", bvh->group[i][j]->type);
		// 	}

		// }	
		i ++;
	}
}


t_bvh   *init_bvh(t_data *data)
{
    t_bvh       *bvh;
	int			idx_c;

    bvh = aligned_alloc(32, sizeof(t_bvh)); // aligned_alloc for opti !!!
	bvh->depth[0] = 0;
	get_group_size(data, bvh);
	copy_bvh_objects(data, bvh);
	data->bvh_geo_data = create_obj_geo_data(bvh);
	get_bbox_min_max(bvh, 0);
	idx_c = cut_into_child_nodes(bvh, 0);
	bvh->childs[idx_c] = -2;
	print_bvh_stats(bvh);
	free_bvh_1(bvh);
	free(data->bvh_geo_data);
    // print_nodes(bvh, 0);
	return (bvh);
}
t_obj_geo *create_obj_geo_data(t_bvh *bvh)
{
	t_object 	*obj;
	t_obj_geo 	*geo_data;
	int		i;

	geo_data = malloc(bvh->group_size[0] * sizeof(t_obj_geo));
	bvh->obj_geo[0] = malloc(sizeof(t_obj_geo *) * bvh->group_size[0]);
	i = -1;
	while (++i < bvh->group_size[0])
	{
		obj = bvh->group[0][i];
		if (obj->type == SPHERE)
			geo_data[i] = extract_geo_data_sphere(obj);
		if (obj->type == TRI)
			geo_data[i] = extract_geo_data_tri(obj);
		bvh->obj_geo[0][i] = &geo_data[i];	
	}
	return (geo_data);
}
t_obj_geo extract_geo_data_sphere(t_object *obj)
{
	t_obj_geo 	geo;
	int			i;

	i = -1;
	while (++i < 3)
	{
		geo.center[i] = obj->geo.sph.center[i];
		geo.bmin[i] = obj->geo.sph.center[i] - obj->geo.sph.radius;
		geo.bmax[i] = obj->geo.sph.center[i] + obj->geo.sph.radius;
	}
	return (geo);
}

t_obj_geo extract_geo_data_tri(t_object *obj)
{
	t_obj_geo 	geo;
	int			i;

	i = -1;
	while (++i < 3)
	{
		geo.center[i] = (obj->geo.tri.v0[i] + obj->geo.tri.v1[i] + obj->geo.tri.v2[i]) / 3; // or the mid of min/max instead ??
		geo.bmin[i] = fmin(obj->geo.tri.v0[i] , fmin(obj->geo.tri.v1[i], obj->geo.tri.v2[i]));
		geo.bmax[i] = fmax(obj->geo.tri.v0[i] , fmax(obj->geo.tri.v1[i], obj->geo.tri.v2[i]));
	}
	return (geo);
}



int	cut_into_child_nodes(t_bvh *bvh, int idx)
{
	static int  idx_c = 8;
	int 		i;
	
	if (bvh->group_size[idx] <= MAX_BVH_GROUP || bvh->depth[idx] == BVH_DEPTH_MAX)
	{
		bvh->childs[idx] = -1;
		return (0);
	}
	cut_in_two(bvh, idx, idx_c, 2);
	bvh->childs[idx] = idx_c;
	i = -1;
	while (++i < 8)
	{
		if (bvh->group_size[bvh->childs[idx] + i] == bvh->group_size[idx])
		{
			bvh->childs[idx] = -1;
			return (idx_c);
		}
	}
	idx_c += 8;
	i = -1;
	while (++i < 8)
	{
		bvh->depth[bvh->childs[idx] + i] = bvh->depth[idx] + 1;
		cut_into_child_nodes(bvh, bvh->childs[idx] + i);
	}
	return (idx_c);
}
void get_child_sizes(t_bvh *bvh, int idx, float mid[3], int idx_left, int idx_right)
{
	int		j;
	int 	i;
	int 	group_size_idx;
	int		n_obj_l[3];
	int		n_obj_r[3];
	int		sum[3];
	int 	axis; 


	group_size_idx = bvh->group_size[idx];
	ft_memset(n_obj_l, 0, sizeof(float[3]));
	ft_memset(n_obj_r, 0, sizeof(float[3]));
	j = -1;
	while (++j < 3)
	{
		i = -1;
		while (++i < group_size_idx) 
		{
			if (bvh->obj_geo[idx][i]->bmin[j] <= mid[j])
				n_obj_l[j]++;
			if (bvh->obj_geo[idx][i]->bmax[j] >= mid[j])
				n_obj_r[j]++;
		}
		sum[j] = n_obj_l[j] + n_obj_r[j];
	}
	if (sum[0] <= sum[1] && sum[0] <= sum[2])
		axis = 0;
	else if (sum[1] <= sum[0] && sum[1] <= sum[2])
		axis = 1;
	else
		axis = 2;

	bvh->group_size[idx_left] = n_obj_l[axis];
	bvh->group_size[idx_right] = n_obj_r[axis];

	// if (bvh->group_size[idx_left] == group_size_idx && bvh->group_size[idx_right] == group_size_idx)
	// 	printf("unsucessful cutting => contact your dev team, it should try another axis...\n");

	// then assigne the elements to the right/left groups
	t_object	**old_group;
	t_obj_geo	**old_geo;
	int			i_left;
	int			i_right;

	old_group = bvh->group[idx];
	old_geo = bvh->obj_geo[idx];
	bvh->group[idx_left] = malloc(sizeof(t_object *) * bvh->group_size[idx_left]);
	bvh->group[idx_right] = malloc(sizeof(t_object *) * bvh->group_size[idx_right]);
	bvh->obj_geo[idx_left] = malloc(sizeof(t_obj_geo *) * bvh->group_size[idx_left]);
	bvh->obj_geo[idx_right] = malloc(sizeof(t_obj_geo *) * bvh->group_size[idx_right]);
	
	i = -1;
	i_left = 0;
	i_right = 0;
	while (++i < group_size_idx) 
	{
		if (old_geo[i]->bmin[axis] <= mid[axis])  // with bbbox for strict !!! 
		{	
			bvh->group[idx_left][i_left] = old_group[i];
			bvh->obj_geo[idx_left][i_left++] = old_geo[i];
		}
		if (old_geo[i]->bmax[axis] >= mid[axis])  // with bbbox for strict !!! 
		{	
			bvh->group[idx_right][i_right] = old_group[i];
			bvh->obj_geo[idx_right][i_right++] = old_geo[i];
		}
	}
	get_bbox(bvh, idx_right, idx, mid[axis], axis, 1);
	get_bbox(bvh, idx_left, idx, mid[axis], axis, 0);
	if (idx == idx_left)
	{
		free(old_group);
		free(old_geo);
	}
}

void get_bbox(t_bvh *bvh, int idx_child, int idx_parent, float mid, int axis, int side)
{
	t_bbox 	bbox_node;
	t_bbox	bbox_elem;

	bbox_node = get_bbox_node(bvh, idx_parent, mid, axis, side);
	bbox_elem = get_bbox_elem(bvh, idx_child);
	save_bbox_min(bvh, bbox_node, bbox_elem, idx_child);
}

t_bbox get_bbox_node(t_bvh *bvh, int idx, float mid, int axis, int side)
{
	t_bbox	res;

	res.min[0] = bvh->min_x[idx];
	res.min[1] = bvh->min_y[idx];
	res.min[2] = bvh->min_z[idx];
	res.max[0] = bvh->max_x[idx];
	res.max[1] = bvh->max_y[idx];
	res.max[2] = bvh->max_z[idx];

	if (side == 0)
		res.max[axis] = mid;
	else if (side == 1)
		res.min[axis] = mid;

	return (res);
}
t_bbox get_bbox_elem(t_bvh *bvh, int idx)
{
	t_bbox	res;

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
		while (++j < bvh->group_size[idx])
		{
			tmp_min = bvh->obj_geo[idx][j]->bmin[i]; 
			if (tmp_min < min)
				min = tmp_min;

			tmp_max = bvh->obj_geo[idx][j]->bmax[i]; 
			if (tmp_max > max)
				max = tmp_max; 
		}
		res.min[i] = min;
		res.max[i] = max;
	}
	return (res);
}
void save_bbox_min(t_bvh *bvh, t_bbox bbox_node, t_bbox bbox_elem, int idx)
{
	bvh->min_x[idx] = fmax(bbox_node.min[0], bbox_elem.min[0]);
	bvh->min_y[idx] = fmax(bbox_node.min[1], bbox_elem.min[1]);
	bvh->min_z[idx] = fmax(bbox_node.min[2], bbox_elem.min[2]);
	bvh->max_x[idx] = fmin(bbox_node.max[0], bbox_elem.max[0]);
	bvh->max_y[idx] = fmin(bbox_node.max[1], bbox_elem.max[1]);
	bvh->max_z[idx] = fmin(bbox_node.max[2], bbox_elem.max[2]);
}

void	cut_in_two(t_bvh *bvh, int idx, int idx_c, int i)
{
		float 	mid[3];
		int		idx_left;
		int		idx_right;

		// get_bbox_min_max(bvh, idx);
		if (i == -1)
			return ;
		
		if (i == 2)
			idx_left = idx_c; // not inplace if depth == 0 ;)
		else
			idx_left = idx;		// else inplace !...
		idx_right = idx_left + pow(2, i);

		mid[0] = find_cutting_plane(bvh, idx, 0);
		mid[1] = find_cutting_plane(bvh, idx, 1);
		mid[2] = find_cutting_plane(bvh, idx, 2);
		
		get_child_sizes(bvh, idx, mid, idx_left, idx_right); // in place ... 
		// create_child_groups(bvh, idx, axis, mid, idx_left, idx_right); // in place ...
		cut_in_two(bvh, idx_left, idx_c, i - 1);
		cut_in_two(bvh, idx_right, idx_c, i - 1);
}


int find_largest_axis(t_bvh *bvh, int idx)
{
	int 	axis;
	float	max;
	float	tmp;

	axis = 0;
	max = 0;
	// x ...
	tmp = bvh->max_x[idx] - bvh->min_x[idx];
	if (tmp > max)
	{
		max = tmp;
		axis = 0;
	}
	//y ...
	tmp = bvh->max_y[idx] - bvh->min_y[idx];
	if (tmp > max)
	{
		max = tmp;
		axis = 1;
	}
	// z ...
	tmp = bvh->max_z[idx] - bvh->min_z[idx];
	if (tmp > max)
	{
		max = tmp;
		axis = 2;
	}
	return (axis);
}

float	find_cutting_plane(t_bvh *bvh, int idx, int axis)
{
	float 	*centers = malloc(sizeof(float) * bvh->group_size[idx]); // or custom find_median !!!!
	int		i;
	float	res;

	i = -1;
	while (++i < bvh->group_size[idx])
		centers[i] = bvh->obj_geo[idx][i]->center[axis];
	res = findMedian(centers, bvh->group_size[idx]);	
	free(centers);
	return(res);
}

void	get_bbox_min_max(t_bvh *bvh, int idx)
{
	// get pt_min / pt_max
	int		j;
	float	min;
	float	max;
	float	tmp_min;
	float	tmp_max;

	// x !!!
	min = FLT_MAX;
	max = -FLT_MAX;
	j = -1;
	while (++j < bvh->group_size[idx])
	{
		tmp_min = bvh->obj_geo[idx][j]->bmin[0]; 
		if (tmp_min < min)
			min = tmp_min;

		tmp_max = bvh->obj_geo[idx][j]->bmax[0]; 
		if (tmp_max > max)
			max = tmp_max; 
	}
	bvh->min_x[idx] = min;
	bvh->max_x[idx] = max;

	// y !!!
	min = FLT_MAX;
	max = -FLT_MAX;
	j = -1;
	while (++j < bvh->group_size[idx])
	{
		tmp_min = bvh->obj_geo[idx][j]->bmin[1]; 
		if (tmp_min < min)
			min = tmp_min;

		tmp_max = bvh->obj_geo[idx][j]->bmax[1]; 
		if (tmp_max > max)
			max = tmp_max; 
	}
	bvh->min_y[idx] = min;
	bvh->max_y[idx] = max;

	// z !!!
	min = FLT_MAX;
	max = -FLT_MAX;
	j = -1;
	while (++j < bvh->group_size[idx])
	{
		tmp_min = bvh->obj_geo[idx][j]->bmin[2]; 
		if (tmp_min < min)
			min = tmp_min;

		tmp_max = bvh->obj_geo[idx][j]->bmax[2]; 
		if (tmp_max > max)
			max = tmp_max; 
	}
	bvh->min_z[idx] = min;
	bvh->max_z[idx] = max;
}


void	update_group(t_data *data, t_bvh *bvh)
{
	t_object	*new_objects;
	int			plane_counter;
	int			i;
	int			j;

	i = 0;
	plane_counter = 0;
	while (i < data->n_obj)
	{
		if (data->objects[i].type != SPHERE && data->objects[i].type != TRI)
			plane_counter++;
		i++;
	}
	new_objects = aligned_alloc(64, sizeof(t_object) * (plane_counter + 1));
	new_objects[0].type = BVH;
	new_objects[0].geo.bvh = (void *)bvh;
	i = 0;
	j = 1;
	while (i < data->n_obj)
	{
		if (data->objects[i].type != SPHERE && data->objects[i].type != TRI)
			ft_memcpy(&new_objects[j++], &data->objects[i], sizeof(t_object));
		i++;
	}
	data->all_objects = data->objects;
	data->objects = new_objects;
	data->n_obj = plane_counter + 1;
}