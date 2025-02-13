#include "minirt.h"

void start_timer(clock_t *start)
{
    *start = clock();
}

double stop_timer(clock_t start)
{
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

void print_render_stats(double render_time)
{
    printf("Render time                                     : %.3f sec\n", render_time);
    printf("Total number of objects                        : %d\n", total_objects);
    printf("Total number of primary rays                   : %d\n", num_primary_rays);
    printf("Total number of objects tests                  : %d\n", num_object_tests);
    printf("Total number of object intersections           : %d\n", num_object_intersections);
}