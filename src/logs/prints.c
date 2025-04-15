/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:39:30 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/15 20:42:35 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
