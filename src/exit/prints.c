
#include "minirt.h"

static void	print_top_stats(void);
static void	get_bvh_stats(t_bvh *bvh, t_bvh_stats *stats);

void	print_intro(void)
{
	printf("%s\n", PINK_TXT_START);
	printf("__/\\\\\\\\____________/\\\\\\\\______________________________");
	printf("/\\\\\\\\\\\\\\\\\\______/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\_        \n");
	printf(" _\\/\\\\\\\\\\\\________/\\\\\\\\\\\\____________________________");
	printf("/\\\\\\///////\\\\\\___\\///////\\\\\\/////__       \n");
	printf("  _\\/\\\\\\//\\\\\\____/\\\\\\//\\\\\\__/\\\\\\_____________");
	printf("___/\\\\\\_\\/\\\\\\_____\\/\\\\\\_________\\/\\\\\\_______      \n");
	printf("   _\\/\\\\\\\\///\\\\\\/\\\\\\/__\\/\\\\\\_\\///___/\\\\");
	printf("/\\\\\\\\\\\\___\\///__\\/\\\\\\\\\\\\\\\\\\\\\\/_________\\/\\\\");
	printf("\\_______     \n");
	printf("    _\\/\\\\\\__\\///\\\\\\/___\\/\\\\\\__/\\\\\\_\\/\\\\\\//");
	printf("//\\\\\\___/\\\\\\_\\/\\\\\\//////\\\\\\__________\\/\\\\\\_______    \n");
	printf("     _\\/\\\\\\____\\///_____\\/\\\\\\_\\/\\\\\\_\\/\\\\\\__\\");
	printf("//\\\\\\_\\/\\\\\\_\\/\\\\\\____\\//\\\\\\_________\\/\\\\\\_______   \n");
	printf("      _\\/\\\\\\_____________\\/\\\\\\_\\/\\\\\\_\\/\\\\\\__");
	printf("_\\/\\\\\\_\\/\\\\\\_\\/\\\\\\_____\\//\\\\\\________\\/\\\\\\_______  \n");
	printf("       _\\/\\\\\\_____________\\/\\\\\\_\\/\\\\\\_\\/\\\\\\");
	printf("___\\/\\\\\\_\\/\\\\\\_\\/\\\\\\______\\//\\\\\\_______\\/\\\\\\_______ \n");
	printf("        _\\///______________\\///__\\///__\\///__");
	printf("__\\///__\\///__\\///________\\///________\\///________\n");
	printf("%s\n", COLOR_END);
	print_top_stats();
}

static void	print_top_stats(void)
{
	printf("%s\n+=============================================================",
		CYAN_TXT_START);
	printf("=================================+\n");
	printf("|                                           STATS               ");
	printf("                               |\n");
	printf("+===============================================================");
	printf("===============================+\n");
	printf("+===============================================================");
	printf("===============================+%s\n\n", COLOR_END);
}

void	print_outro(void)
{
	printf("%s\n+============================================================",
		CYAN_TXT_START);
	printf("==================================+\n");
	printf("+================================================================");
	printf("==============================+\n");
	printf("|                                           BYE!!                ");
	printf("                              |\n");
	printf("+==============================================================");
	printf("================================+%s\n", COLOR_END);
}

void	print_bvh_stats(t_bvh *bvh)
{
	t_bvh_stats	stats;

	if (bvh->childs[0] != -2)
	{
		get_bvh_stats(bvh, &stats);
		printf("%s  [BVH]\n\n", CYAN_TXT_START);
		printf("    > max depth of %d\n\n", stats.max_depth);
		printf("    > %d n_obj at leafs (%.0f%%)\n\n", 
			stats.n_obj, (float)stats.n_obj / bvh->group_size[0] * 100);
		printf("    > %.2f obj/leafs (min : %d / max : %d)%s\n",
			(float)stats.n_obj / stats.n_leafs, stats.min, stats.max,
			COLOR_END);
	}
}

static void	get_bvh_stats(t_bvh *bvh, t_bvh_stats *stats)
{
	int	i;

	ft_memset(stats, 0, sizeof(t_bvh_stats));
	stats->min = INT_MAX;
	i = -1;
	while (bvh->childs[++i] != -2)
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

void	print_bvh_build_t(struct timeval t_start, struct timeval t_end)
{
	float	t;

	t = (t_end.tv_sec - t_start.tv_sec) * 1000 + (t_end.tv_usec - t_start.tv_usec) * 0.001; 
	printf("%s    > %.2f ms to build BVH\n", CYAN_TXT_START, t);
	printf("\n  [RENDER]%s\n", COLOR_END);
}

void	print_img_render_t(struct timeval t_start, struct timeval t_end)
{
	static int		n_img;
	float			t;

	t = (t_end.tv_sec - t_start.tv_sec) * 1000 + (t_end.tv_usec - t_start.tv_usec) * 0.001; 
	printf("%s    > img %d rendered after %.1f ms%s\n", CYAN_TXT_START, n_img++, t, COLOR_END);
}

void	print_tri_count(int counter)
{
	printf("%s  [TRIMESH]\n\n", CYAN_TXT_START);
	printf("    > %d tris%s\n\n", counter, COLOR_END);
}
