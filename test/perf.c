#include "minirt.h"

void start_timer(clock_t *start)
{
    *start = clock();
}

double stop_timer(clock_t start)
{
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

double get_time()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9; // Convert to seconds
}

void print_render_stats(double render_time)
{
    printf("Render time                                    : %.3f sec\n", render_time);
    printf("Total number of objects                        : %d\n", total_objects);
    printf("Total number of primary rays                   : %d\n", num_primary_rays);
    printf("Rays per second                                : %.3f M rays/sec\n", num_primary_rays / (1e6 * render_time));
    printf("Total number of objects tests                  : %d\n", num_object_tests);
    printf("Total number of object intersections           : %d\n", num_object_intersections);
    printf("Avg. object tests per primary ray              : %.3f\n", (double)num_object_tests / num_primary_rays);
    printf("BVH efficiency (intersections/tests)           : %.3f%%\n", (num_object_intersections * 100.0) / num_object_tests);
    printf("Render time breakdown:\n");
    printf("  - Primary Rays Computation         : %.3f sec (%.2f%%)\n", time_primary_rays, (time_primary_rays / time_total_render) * 100);
    printf("  - Visibility Tests Computation     : %.3f sec (%.2f%%)\n", time_visibility_test, (time_visibility_test / time_total_render) * 100);
    printf("  - Normal Intersect Computation     : %.3f sec (%.2f%%)\n", time_normal_intersect, (time_normal_intersect / time_total_render) * 100);
    printf("  - Shading Computation              : %.3f sec (%.2f%%)\n", time_shading, (time_shading / time_total_render) * 100);
    printf("  - Total Render Time                : %.3f sec\n", time_total_render);

}