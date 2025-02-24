#include "minirt.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <stdint.h>

// Function to interleave bits of x, y, z coordinates for Morton encoding
uint32_t morton_encode(float x, float y, float z) {
    // You may want to scale and shift the values of x, y, z into an appropriate integer range.
    // Assuming coordinates are in range [0, 1], you could scale to a 32-bit integer range.

    uint32_t X = (uint32_t)(x * 1024) & 0x3FF;
    uint32_t Y = (uint32_t)(y * 1024) & 0x3FF;
    uint32_t Z = (uint32_t)(z * 1024) & 0x3FF;

    uint32_t morton = 0;
    for (int i = 0; i < 10; ++i) {
        morton |= ((X >> i) & 1) << (3 * i);
        morton |= ((Y >> i) & 1) << (3 * i + 1);
        morton |= ((Z >> i) & 1) << (3 * i + 2);
    }
    return morton;
}



int compare_morton(const void *a, const void *b) {
    t_object *objA = (t_object *)a;
    t_object *objB = (t_object *)b;
	uint32_t mortonA, mortonB;

    if (objA->type == PLANE)
		mortonA = morton_encode(objA->geo.pl.point[0], objA->geo.pl.point[1], objA->geo.pl.point[2]);
    else
		mortonA = morton_encode(objA->geo.sph.center[0], objA->geo.sph.center[1], objA->geo.sph.center[2]);
	 

	if (objB->type == PLANE)
		mortonB = morton_encode(objB->geo.pl.point[0], objB->geo.pl.point[1], objB->geo.pl.point[2]);
    else
		mortonB = morton_encode(objB->geo.sph.center[0], objB->geo.sph.center[1], objB->geo.sph.center[2]);

    if (mortonA < mortonB)
        return -1;
    if (mortonA > mortonB)
        return 1;
    return 0;
}

void sort_by_morton(t_data *data) {
    // Sort the objects array based on Morton codes
    qsort(data->objects, data->n_obj, sizeof(t_object), compare_morton);
}


void generate_random_spheres(t_data *data, int n) {
    srand(123456);//time(NULL));
    for (int i = 1; i < n + 1; i++){
        data->objects[i].geo.sph.center[0] = ((float)rand() / RAND_MAX) * 100 - 50;
        data->objects[i].geo.sph.center[1]  = 0.1;
        data->objects[i].geo.sph.center[2]  = -((float)rand() / RAND_MAX) * 200 + 15;
        data->objects[i].geo.sph.radius = 0.1;
        data->objects[i].type = SPHERE;
        data->objects[i].mat.checker_flag = 0;
        data->objects[i].mat.rgb[0] = rand() % 256;
        data->objects[i].mat.rgb[1] = rand() % 256;
        data->objects[i].mat.rgb[2] = rand() % 256;
        data->objects[i].mat.refl_coeff = ((float)rand() / RAND_MAX);
		data->objects[i].mat.refr_coeff = 0 ;
		data->objects[i].mat.refr_idx 	= 1 ;
    }
}



void	parsing(t_data *data)
{
	// t_sphere		*sph_1 = malloc(sizeof(t_sphere));
	// t_sphere sph_1_data = {{-4, 0.5, 0}, 0.5};	
	// ft_memcpy(sph_1, &sph_1_data, sizeof(t_sphere));	
	// data->objects[0].type = SPHERE;
	// data->objects[0].geo = sph_1;
	// data->objects[0].mat.rgb[0] = 255;
	// data->objects[0].mat.rgb[1] = 255;
	// data->objects[0].mat.rgb[2] = 255;
	// data->objects[0].mat.refl_coeff = 0.0;	
	
	
	t_camera		cam = {70, {0, 1, 15}, {0,0,-1}};
	t_ambient_light	ambient = {0.2, {1, 1 , 1}}; // normalized light color
	// t_light lgt_1_data = {0.6, {5, 5, 0}, {255.0/255, 255.0/255, 255.0/255}};
	t_light lgt_1_data = {0.6, {5, 3, 20}, {255.0/255, 255.0/255, 255.0/255}};

	data->cam = cam;
	data->ambient = ambient;
	// LIGHTS
	data->lights = malloc(sizeof(t_light) * 3);
	data->lights[0] = lgt_1_data;
	data->lights[1].brightness = -1;

	//OBJECTS
	int 	n = 60000;
	data->n_obj = n + 2;
	data->objects = aligned_alloc(64, sizeof(t_object) * (n + 2));
	generate_random_spheres(data, n);


	// PLANE
	data->objects[0].geo.pl.point[0] = 0;
	data->objects[0].geo.pl.point[1] = 0;
	data->objects[0].geo.pl.point[2] = 0;
	data->objects[0].geo.pl.normal[0] = 0;
	data->objects[0].geo.pl.normal[1] = 1;
	data->objects[0].geo.pl.normal[2] = 0;
	data->objects[0].geo.pl.u[0] = 1;
	data->objects[0].geo.pl.u[1] = 0;
	data->objects[0].geo.pl.u[2] = 0;

	data->objects[0].geo.pl.v[0] = 0;
	data->objects[0].geo.pl.v[1] = 0;
	data->objects[0].geo.pl.v[2] = -1;

	data->objects[0].type = PLANE;
	data->objects[0].mat.checker_flag = 1;
	data->objects[0].mat.rgb[0] = 255;
	data->objects[0].mat.rgb[1] = 0;
	data->objects[0].mat.rgb[2] = 0;
	data->objects[0].mat.rgb2[0] = 255;
	data->objects[0].mat.rgb2[1] = 255;
	data->objects[0].mat.rgb2[2] = 0;
	data->objects[0].mat.refl_coeff = 0;
	data->objects[0].mat.refr_coeff = 0;
	data->objects[0].mat.refr_idx 	= 1;	


	//SPHERE
	data->objects[n + 1].geo.sph.center[0] = 0;
	data->objects[n + 1].geo.sph.center[1] = 2;
	data->objects[n + 1].geo.sph.center[2] = 4;
	data->objects[n + 1].geo.sph.radius = 2;
	data->objects[n + 1].type = SPHERE;
	data->objects[n + 1].mat.checker_flag = 0;
	data->objects[n + 1].mat.rgb[0] = 0;
	data->objects[n + 1].mat.rgb[1] = 0;
	data->objects[n + 1].mat.rgb[2] = 0;
	data->objects[n + 1].mat.refl_coeff = 0.0 ;
	data->objects[n + 1].mat.refr_coeff = 0.95 ;
	data->objects[n + 1].mat.refr_idx 	= 1.5 ;	

	sort_by_morton(data);

	// t_cylinder *sph2 = malloc(sizeof(t_cylinder));
	// sph2->center[0]  = 2;
	// sph2->center[1]  = 2;
	// sph2->center[2]  = 10;
	// sph2->radius = 0.5;
	// sph2->height = 4;
	// sph2->dir[0] = 1;
	// sph2->dir[1] = 1;
	// sph2->dir[2] = -1;
	// normalize(sph2->dir);
	// data->objects[n + 2].type = CYLINDER;
	// data->objects[n + 2].mat.checker_flag = 0;
	// data->objects[n + 2].geo = sph2;
	// data->objects[n + 2].mat.rgb[0] = 255;
	// data->objects[n + 2].mat.rgb[1] = 0;
	// data->objects[n + 2].mat.rgb[2] = 0;
	// data->objects[n + 2].mat.refl_coeff = 0 ; //((float)rand() / RAND_MAX);
	// data->objects[n + 2].mat.refr_coeff = 0 ;
	// data->objects[n + 2].mat.refr_idx 	= 1.5 ;	

	// t_plane			*pl_2 = malloc(sizeof(t_plane));
	// t_plane pl_2_data = {{0, 0, -300}, {0, 0, 1}};
	// ft_memcpy(pl_2, &pl_2_data, sizeof(t_plane));
	// data->objects[1].type = PLANE;
	// data->objects[1].geo = pl_2;
	// data->objects[1].mat.rgb[0] = 00;
	// data->objects[1].mat.rgb[1] = 200;
	// data->objects[1].mat.rgb[2] = 00;
	// data->objects[1].mat.refl_coeff = 0;
}
