#include "minirt.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void generate_random_spheres(t_data *data, int n) {
    srand(time(NULL));
    for (int i = 1; i < n + 1; i++) {
        t_sphere *sph = malloc(sizeof(t_sphere));
        sph->center[0] = ((float)rand() / RAND_MAX) * 10 - 5;
        sph->center[1]  = 0.1;
        sph->center[2]  = ((float)rand() / RAND_MAX) * 30 - 15;
        sph->radius = 0.1;
        data->objects[i].type = SPHERE;
        data->objects[i].geo = sph;
        data->objects[i].mat.rgb[0] = rand() % 256;
        data->objects[i].mat.rgb[1] = rand() % 256;
        data->objects[i].mat.rgb[2] = rand() % 256;
        data->objects[i].mat.refl_coeff = 0 ; //((float)rand() / RAND_MAX);
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
	t_light lgt_1_data = {0.6, {500, 2000.05, 10215}, {255.0/255, 255.0/255, 255.0/255}};
	

	data->cam = cam;
	data->ambient = ambient;
	// LIGHTS
	data->lights = malloc(sizeof(t_light) * 3);
	data->lights[0] = lgt_1_data;
	data->lights[1].brightness = -1;

	//OBJECTS
	int 	n = 200;
	data->n_obj = n + 2;
	data->objects = malloc(sizeof(t_object) * (n + 2));
	generate_random_spheres(data, n);

	t_plane			*pl_1 = malloc(sizeof(t_plane));
	t_plane pl_1_data = {{0, 0, 0}, {0, 1, 0}};
	ft_memcpy(pl_1, &pl_1_data, sizeof(t_plane));
	data->objects[0].type = PLANE;
	data->objects[0].geo = pl_1;
	data->objects[0].mat.rgb[0] = 100;
	data->objects[0].mat.rgb[1] = 100;
	data->objects[0].mat.rgb[2] = 100;
	data->objects[0].mat.refl_coeff = 0;
	data->objects[0].mat.refr_coeff = 0;
	data->objects[0].mat.refr_idx 	= 1;	


	t_sphere *sph = malloc(sizeof(t_sphere));
	sph->center[0]  = 0;
	sph->center[1]  = 2;
	sph->center[2]  = 0;
	sph->radius = 2;
	data->objects[n + 1].type = SPHERE;
	data->objects[n + 1].geo = sph;
	data->objects[n + 1].mat.rgb[0] = 100;
	data->objects[n + 1].mat.rgb[1] = 100;
	data->objects[n + 1].mat.rgb[2] = 100;
	data->objects[n + 1].mat.refl_coeff = 0 ; //((float)rand() / RAND_MAX);
	data->objects[n + 1].mat.refr_coeff = 0.8 ;
	data->objects[n + 1].mat.refr_idx 	= 1.5 ;	

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
