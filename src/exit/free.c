#include "minirt.h"

/* obj.geo and non-leaf groups -> not needed after bvh creation anymore.*/
void    free_bvh_1(t_bvh *bvh)
{
    int i;

    // if ((bvh->childs[0] != -2))
    // {
    //     free(bvh->group[0]);
    //     free(bvh->obj_geo[0]);
    // }
    // else
    //     return ;
    i = -1;
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

    i = -1;
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

void    free_obj_parse_1(t_obj_parser *parsers, int n_files)
{
    int i;

    i = -1;
    while (++i < n_files)
        free(parsers[i].filename);
    free(parsers);
}

void    free_obj_parse_2(t_obj_parser *parsers, int n_files)
{
    int i;

    i = -1;
    while (++i < n_files)
    {
        free(parsers[i].filename);
        free(parsers[i].vertices);
        free(parsers[i].normals);
    }
    free(parsers);
}

