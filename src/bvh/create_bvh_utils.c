#include "minirt.h"

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

void	create_obj_list_root(t_data *data, t_bvh *bvh)
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
