#include "minirt.h"

void	parsing(t_data *data)
{
	t_sphere		*sph_1;
	t_sphere		*sph_2;
	t_sphere		*sph_3;
	t_sphere		*sph_4;
	t_sphere		*sph_5;
	t_sphere		*sph_6;
	t_sphere		*sph_7;
	t_sphere		*sph_8;
	t_sphere		*sph_9;
	t_sphere		*sph_10;
	t_plane			*pl_1;
	t_camera		cam = {90, {0, 1.7, 10}, {0,0,-1}};
	t_ambient_light	ambient = {0.2, {255, 255, 255}};

	sph_1 = malloc(sizeof(t_sphere));
	sph_2 = malloc(sizeof(t_sphere));
		sph_3 = malloc(sizeof(t_sphere));
	sph_4 = malloc(sizeof(t_sphere));
		sph_5 = malloc(sizeof(t_sphere));
	sph_6 = malloc(sizeof(t_sphere));
		sph_7 = malloc(sizeof(t_sphere));
	sph_8 = malloc(sizeof(t_sphere));
		sph_9 = malloc(sizeof(t_sphere));
	sph_10 = malloc(sizeof(t_sphere));
	pl_1 = malloc(sizeof(t_plane));

	t_sphere sph_1_data = {{0, 1, 0}, 1.0, {255, 0, 0}};
	t_sphere sph_2_data = {{0.5, 0.5, 2}, 0.5, {0, 0, 255}};
	t_sphere sph_3_data = {{0, 3, 0}, 1.0, {155, 0, 0}};
	t_sphere sph_4_data = {{1.5, 1.5, 2.2}, 0.5, {0, 50, 255}};
	t_sphere sph_5_data = {{0, 1.5, 0}, 1.0, {255, 50, 0}};
	t_sphere sph_6_data = {{0.5, 0.5, -6.5}, 2.5, {100, 0, 255}};
	t_sphere sph_7_data = {{3, 1, 0}, 2.0, {255, 50, 0}};
	t_sphere sph_8_data = {{2.5, 4.5, 2}, 1.5, {100, 0, 255}};
	t_sphere sph_9_data = {{0, 1, -1}, 1.5, {255, 60, 20}};
	t_sphere sph_10_data = {{0.5, -0.5, -2}, 0.5, {100, 0, 255}};



	t_plane pl_1_data = {{0, 0, 0}, {0, 1, 0}, {100, 100, 100}};
	t_light lgt_1_data = {0.6, {0, 10, 0}, {255, 255, 255}};

	ft_memcpy(sph_1, &sph_1_data, sizeof(t_sphere));
	ft_memcpy(sph_2, &sph_2_data, sizeof(t_sphere));
	ft_memcpy(sph_3, &sph_3_data, sizeof(t_sphere));
	ft_memcpy(sph_4, &sph_4_data, sizeof(t_sphere));
	ft_memcpy(sph_5, &sph_5_data, sizeof(t_sphere));
	ft_memcpy(sph_6, &sph_6_data, sizeof(t_sphere));
	ft_memcpy(sph_7, &sph_7_data, sizeof(t_sphere));
	ft_memcpy(sph_8, &sph_8_data, sizeof(t_sphere));
	ft_memcpy(sph_9, &sph_9_data, sizeof(t_sphere));
	ft_memcpy(sph_10, &sph_10_data, sizeof(t_sphere));
	ft_memcpy(pl_1, &pl_1_data, sizeof(t_plane));

	data->cam = cam;
	data->ambient = ambient;
	data->objects = malloc(sizeof(t_object) * 12);
	data->objects[0].type = SPHERE;
	data->objects[0].data = sph_1;
	data->objects[1].type = SPHERE;
	data->objects[1].data = sph_2;
	data->objects[2].type = SPHERE;
	data->objects[2].data = sph_3;
	data->objects[3].type = SPHERE;
	data->objects[3].data = sph_4;
	data->objects[4].type = SPHERE;
	data->objects[4].data = sph_5;
	data->objects[5].type = SPHERE;
	data->objects[5].data = sph_6;
	data->objects[6].type = SPHERE;
	data->objects[6].data = sph_7;
	data->objects[7].type = SPHERE;
	data->objects[7].data = sph_8;
	data->objects[8].type = SPHERE;
	data->objects[8].data = sph_9;
	data->objects[9].type = SPHERE;
	data->objects[9].data = sph_10;
	data->objects[10].type = PLANE;
	data->objects[10].data = pl_1;
	data->objects[11].type = END;
	
	data->lights = malloc(sizeof(t_light) * 2);
	data->lights[0] = lgt_1_data;
	data->lights[1].brightness = -1;
}
