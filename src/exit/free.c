#include "minirt.h"

/* obj.geo and non-leaf groups -> not needed after bvh creation anymore.*/
void    free_bvh_1(t_bvh *bvh)
{
    int i;

    free(bvh->group[0]);
    free(bvh->obj_geo[0]);
    i = 7;
    while (bvh->childs[++i] != -2)
    {
        if (bvh->childs[i] != -1)
            free(bvh->group[i]);
        free(bvh->obj_geo[i]);
    }
}

/* free leaf groups and the bvh itself*/
void    free_bvh_2(t_bvh *bvh)
{
    int i;

    i = 7;
    while (bvh->childs[++i] != -2)
    {
        if (bvh->childs[i] == -1)
            free(bvh->group[i]);
    }
    free(bvh);
}

void    join_threads(t_data *data)
{
    int i;

    data->exit = 1;
    i = -1;
    while (++i < N_THREAD)
    {
        if (pthread_join(data->threads[i], NULL))
            exit(EXIT_FAILURE);
    }
}

/* free data*/
void    free_data(t_data *data)
{
    free(data->all_objects);
    free(data->objects);
    free(data->lights);
    pthread_mutex_destroy(&data->joblist_mutex);
}