#include "minirt.h"

static int	cut_into_child_nodes(t_bvh *bvh, int idx);
static void	cut_in_two(t_bvh *bvh, int idx, int idx_c, int i);
static void	set_cutting_plane_n_obj(t_bvh *bvh, int idx, t_cut_in_two *cut);
static void create_child_groups(t_bvh *bvh, t_cut_in_two *cut);

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
	create_obj_list_root(data, bvh);
	data->bvh_geo_data = create_obj_geo_data(bvh);
	get_bbox_min_max_root(bvh);
	idx_c = cut_into_child_nodes(bvh, 0);
	bvh->childs[idx_c] = -2;
	print_bvh_stats(bvh);
	free_bvh_1(bvh);
	free(data->bvh_geo_data);
	return (bvh);
}


static int	cut_into_child_nodes(t_bvh *bvh, int idx)
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
static void	cut_in_two(t_bvh *bvh, int idx, int idx_c, int i)
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

static void	set_cutting_plane_n_obj(t_bvh *bvh, int idx, t_cut_in_two *cut)
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

static void create_child_groups(t_bvh *bvh, t_cut_in_two *cut)
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
