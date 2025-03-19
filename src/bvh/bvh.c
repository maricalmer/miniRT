#include "minirt.h"


typedef struct
{
	int			axis;
	float		mid[3];
	int			idx_left;
	int			idx_right;
	int			old_n_obj;
	t_object	**old_group;
	t_obj_geo	**old_geo;
}	t_cut_in_two;


void	get_group_size(t_data *data, t_bvh *bvh);
void	copy_bvh_objects(t_data *data, t_bvh *bvh);
t_obj_geo *create_obj_geo_data(t_bvh *bvh);
t_obj_geo extract_geo_data_sphere(t_object *obj);
t_obj_geo extract_geo_data_tri(t_object *obj);
t_bvh   *init_bvh(t_data *data);
int		cut_into_child_nodes(t_bvh *bvh, int idx);
void 	create_child_groups(t_bvh *bvh, t_cut_in_two *cut);
void	cut_in_two(t_bvh *bvh, int idx, int idx_c, int i);
void	get_mid_planes(t_bvh *bvh, int idx, t_cut_in_two *cut);
void	set_cutting_plane_n_obj(t_bvh *bvh, int idx, t_cut_in_two *cut);
int		find_min_idx(int x[3]);
void 	malloc_groups_n_geo(t_bvh *bvh, t_cut_in_two *cut);
void	get_bbox_min_max_root(t_bvh *bvh);
void 	get_bboxes(t_bvh *bvh, int idx, t_cut_in_two *cut);
t_bbox 	get_bbox_node(t_bvh *bvh, int idx, t_cut_in_two *cut, int side);
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

t_bvh   *init_bvh(t_data *data)
{
    t_bvh       *bvh;
	int			idx_c;

    bvh = aligned_alloc(32, sizeof(t_bvh)); // aligned_alloc for opti !!!
	bvh->depth[0] = 0;
	// double check again what initialization is strictly necessary
	ft_memset(bvh->min_x, 0, sizeof(float[8]));
	ft_memset(bvh->min_y, 0, sizeof(float[8]));
	ft_memset(bvh->min_z, 0, sizeof(float[8]));
	ft_memset(bvh->max_x, 0, sizeof(float[8]));
	ft_memset(bvh->max_y, 0, sizeof(float[8]));
	ft_memset(bvh->max_z, 0, sizeof(float[8]));
	ft_memset(bvh->group_size, 0, sizeof(int[8]));
	ft_memset(bvh->group, 0, sizeof(void *) * 8);
	// ft_memset(bvh->group_size, 0, sizeof(int[8]));
	get_group_size(data, bvh);
	copy_bvh_objects(data, bvh);
	data->bvh_geo_data = create_obj_geo_data(bvh);
	get_bbox_min_max_root(bvh);
	idx_c = cut_into_child_nodes(bvh, 0);
	bvh->childs[idx_c] = -2;
	print_bvh_stats(bvh);
	free_bvh_1(bvh);
	free(data->bvh_geo_data);
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
		return (idx_c);
	}
	cut_in_two(bvh, idx, idx_c, 2);
	bvh->childs[idx] = idx_c;
	i = -1;
	while (++i < 8)
		if (bvh->group_size[bvh->childs[idx] + i] == bvh->group_size[idx])
			return (bvh->childs[idx] = -1);
	idx_c += 8;
	i = -1;
	while (++i < 8)
	{
		bvh->depth[bvh->childs[idx] + i] = bvh->depth[idx] + 1;
		cut_into_child_nodes(bvh, bvh->childs[idx] + i);
	}
	return (idx_c);
}


/* recursive function to halve a bvh node in two.
up to depth == 3 ---> cut in height ;)
if depth != 0, then the cutting occurs inplace in the preallocated SoA BVH..*/
void	cut_in_two(t_bvh *bvh, int idx, int idx_c, int i)
{
	t_cut_in_two	cut;

	if (i == -1)
		return ;
	if (i == 2)
		cut.idx_left = idx_c;
	else
		cut.idx_left = idx;
	cut.idx_right = cut.idx_left + pow(2, i);
	cut.old_n_obj = bvh->group_size[idx];
	cut.old_group = bvh->group[idx];
	cut.old_geo = bvh->obj_geo[idx];
	get_mid_planes(bvh, idx, &cut);
	set_cutting_plane_n_obj(bvh, idx, &cut);
	malloc_groups_n_geo(bvh, &cut);
	create_child_groups(bvh, &cut);
	get_bboxes(bvh, idx, &cut);
	if (idx == cut.idx_left)
	{
		free(cut.old_group);
		free(cut.old_geo);
	}
	cut_in_two(bvh, cut.idx_left, idx_c, i - 1);
	cut_in_two(bvh, cut.idx_right, idx_c, i - 1);
}

void	set_cutting_plane_n_obj(t_bvh *bvh, int idx, t_cut_in_two *cut)
{
	int		i;
	int		j;
	int		n_obj_l[3];
	int		n_obj_r[3];
	int		sum[3];

	ft_memset(n_obj_l, 0, sizeof(float[3]));
	ft_memset(n_obj_r, 0, sizeof(float[3]));
	j = -1;
	while (++j < 3)
	{
		i = -1;
		while (++i < cut->old_n_obj) 
		{
			if (bvh->obj_geo[idx][i]->bmin[j] <= cut->mid[j])
				n_obj_l[j]++;
			if (bvh->obj_geo[idx][i]->bmax[j] >= cut->mid[j])
				n_obj_r[j]++;
		}
		sum[j] = n_obj_l[j] + n_obj_r[j];
	}
	cut->axis = find_min_idx(sum);
	bvh->group_size[cut->idx_left] = n_obj_l[cut->axis];
	bvh->group_size[cut->idx_right] = n_obj_r[cut->axis];
}
int	find_min_idx(int x[3])
{
	if (x[0] <= x[1] && x[0] <= x[2])
		return (0);
	if (x[1] <= x[0] && x[1] <= x[2])
		return (1);
	return (2);
}

void malloc_groups_n_geo(t_bvh *bvh, t_cut_in_two *cut)
{
	bvh->group[cut->idx_left] = malloc(sizeof(t_object *)
								* bvh->group_size[cut->idx_left]);
	bvh->group[cut->idx_right] = malloc(sizeof(t_object *)
								* bvh->group_size[cut->idx_right]);
	bvh->obj_geo[cut->idx_left] = malloc(sizeof(t_obj_geo *)
								* bvh->group_size[cut->idx_left]);
	bvh->obj_geo[cut->idx_right] = malloc(sizeof(t_obj_geo *)
								* bvh->group_size[cut->idx_right]);
}

void create_child_groups(t_bvh *bvh, t_cut_in_two *cut)
{
	int		i;
	int		i_left;
	int		i_right;

	i = -1;
	i_left = 0;
	i_right = 0;
	while (++i < cut->old_n_obj) 
	{
		if (cut->old_geo[i]->bmin[cut->axis] <= cut->mid[cut->axis])
		{	
			bvh->group[cut->idx_left][i_left] = cut->old_group[i];
			bvh->obj_geo[cut->idx_left][i_left++] = cut->old_geo[i];
		}
		if (cut->old_geo[i]->bmax[cut->axis] >= cut->mid[cut->axis])
		{	
			bvh->group[cut->idx_right][i_right] = cut->old_group[i];
			bvh->obj_geo[cut->idx_right][i_right++] = cut->old_geo[i];
		}
	}
}

void get_bboxes(t_bvh *bvh, int idx, t_cut_in_two *cut)
{
	t_bbox 	bbox_node;
	t_bbox	bbox_elem;

	bbox_node = get_bbox_node(bvh, idx, cut, 1);
	bbox_elem = get_bbox_elem(bvh, cut->idx_right);
	save_bbox_min(bvh, bbox_node, bbox_elem, cut->idx_right);

	bbox_node = get_bbox_node(bvh, idx, cut, 0);
	bbox_elem = get_bbox_elem(bvh, cut->idx_left);
	save_bbox_min(bvh, bbox_node, bbox_elem, cut->idx_left);
}

t_bbox get_bbox_node(t_bvh *bvh, int idx, t_cut_in_two *cut, int side)
{
	t_bbox	res;

	res.min[0] = bvh->min_x[idx];
	res.min[1] = bvh->min_y[idx];
	res.min[2] = bvh->min_z[idx];
	res.max[0] = bvh->max_x[idx];
	res.max[1] = bvh->max_y[idx];
	res.max[2] = bvh->max_z[idx];
	if (side == 0)
		res.max[cut->axis] = cut->mid[cut->axis];
	else if (side == 1)
		res.min[cut->axis] = cut->mid[cut->axis];
	return (res);
}

t_bbox get_bbox_elem(t_bvh *bvh, int idx)
{
	t_bbox	res;
	int		i;
	int		j;
	float	min;
	float	max;

	i = -1;
	while (++i < 3)
	{
		min = FLT_MAX;
		max = -FLT_MAX;
		j = -1;
		while (++j < bvh->group_size[idx])
		{
			if (bvh->obj_geo[idx][j]->bmin[i] < min)
				min = bvh->obj_geo[idx][j]->bmin[i];
			if (bvh->obj_geo[idx][j]->bmax[i] > max)
				max = bvh->obj_geo[idx][j]->bmax[i]; 
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

void	get_mid_planes(t_bvh *bvh, int idx, t_cut_in_two *cut)
{
	float 	*centers;
	int		i;
	int		j;

	centers = malloc(sizeof(float) * bvh->group_size[idx]); // or custom find_median !!!!
	j = -1;
	while (++j < 3)
	{
		i = -1;
		while (++i < bvh->group_size[idx])
			centers[i] = bvh->obj_geo[idx][i]->center[j];
		cut->mid[j] = findMedian(centers, bvh->group_size[idx]);
	}
	free(centers);
}

void	get_bbox_min_max_root(t_bvh *bvh)
{
	t_bbox 	bbox1;
	t_bbox	bbox2;

	bbox1 = get_bbox_elem(bvh, 0);
	bbox2 = bbox1;
	save_bbox_min(bvh, bbox1, bbox2, 0);
}

void	update_group(t_data *data, t_bvh *bvh)
{
	t_object	*new_objects;
	int			plane_counter;
	int			i;
	int			j;

	i = -1;
	plane_counter = 0;
	while (++i < data->n_obj)
		if (data->objects[i].type != SPHERE && data->objects[i].type != TRI)
			plane_counter++;
	new_objects = aligned_alloc(64, sizeof(t_object) * (plane_counter + 1));
	new_objects[0].type = BVH;
	new_objects[0].geo.bvh = (void *)bvh;
	i = -1;
	j = 1;
	while (++i < data->n_obj)
		if (data->objects[i].type != SPHERE && data->objects[i].type != TRI)
			ft_memcpy(&new_objects[j++], &data->objects[i], sizeof(t_object));
	data->all_objects = data->objects;
	data->objects = new_objects;
	data->n_obj = plane_counter + 1;
}
