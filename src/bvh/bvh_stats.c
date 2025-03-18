# include <minirt.h>

typedef struct
{
    int max_depth;
    int n_nodes;
    int n_obj;
    int n_leafs;
    int min;
    int max;
}   t_bvh_stats;

static void    get_bvh_stats(t_bvh *bvh, t_bvh_stats *stats);

void    print_bvh_stats(t_bvh *bvh)
{
    t_bvh_stats stats;
    
    if (bvh->childs[0] != -2)
    {
        get_bvh_stats(bvh, &stats);
        printf("  [BVH]\n\n");
        printf("    > max depth of %d\n\n", stats.max_depth);
        printf("    > %d n_obj at leafs (%.0f%%)\n\n", 
                stats.n_obj, (float)stats.n_obj / bvh->group_size[0] * 100);
        printf("    > %.2f obj/leafs (min : %d / max : %d)%s\n",
            (float)stats.n_obj / stats.n_leafs, stats.min, stats.max, COLOR_END);
    }
}

static void    get_bvh_stats(t_bvh *bvh, t_bvh_stats *stats)
{
    int i;

    ft_memset(stats, 0, sizeof(t_bvh_stats));
    stats->min = INT_MAX;
    i = -1;
    while(bvh->childs[++i] != -2)
    {
        if (bvh->childs[i] == -1)
        {
            stats->max_depth = imax(stats->max_depth, bvh->depth[i]);
            stats->n_leafs++;
            stats->n_obj += bvh->group_size[i];
            stats->min = imin(stats->min, bvh->group_size[i]);
            stats->max = imax(stats->max, bvh->group_size[i]);
        }
    }
    stats->n_nodes = i;
}