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
    
    get_bvh_stats(bvh, &stats);
    printf("\n");
    printf("%s+=========================================+\n", CYAN_TXT_START);
    printf("|                BVH STATS                |\n");
    printf("+=========================================+\n");
    printf("+=========================================+\n");
    printf("|                                         |\n");
    printf("| max depth                             %d |\n", stats.max_depth);
    printf("|                                         |\n");
    printf("| n_nodes                           %d |\n", stats.n_nodes);
    printf("|                                         |\n");
    printf("| n_obj at leafs %d (%.0f%%)            |\n", 
            stats.n_obj, (float)stats.n_obj / bvh->group_size[0] * 100);
    printf("|                                         |\n");
    printf("| %.2f obj/leafs (min : %d / max : %d)   |\n",
        (float)stats.n_obj / stats.n_leafs, stats.min, stats.max);
    printf("|                                         |\n");
    printf("+=========================================+%s\n", COLOR_END);
}

static void    get_bvh_stats(t_bvh *bvh, t_bvh_stats *stats)
{
    int i;

    ft_memset(stats, 0, sizeof(t_bvh_stats));
    stats->min = INT_MAX;
    i = 7;
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