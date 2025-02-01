#include "minirt.h"

void	parsing(t_data *data)
{
	t_sphere	*sph_1;
	t_sphere	*sph_2;
	t_plane		*pl_1;
	t_camera	cam = {70, {0, 2, -10}, {0,0,1}};

	
	sph_1 = malloc(sizeof(t_sphere));
	sph_2 = malloc(sizeof(t_sphere));
	pl_1 = malloc(sizeof(t_plane));

	t_sphere sph_1_data = {{0, 1, 0}, 1.0, {255, 0, 0}};
	t_sphere sph_2_data = {{0.5, 0.5, -2}, 0.5, {0, 0, 255}};
	t_plane pl_1_data = {{0, 0, 0}, {1, 0, 0}, {100, 100, 100}};

	ft_memcpy(sph_1, &sph_1_data, sizeof(t_sphere));
	ft_memcpy(sph_2, &sph_2_data, sizeof(t_sphere));
	ft_memcpy(pl_1, &pl_1_data, sizeof(t_plane));

	data->cam = cam;
	data->objects = malloc(sizeof(t_object) * 4);
	data->objects[0].type = SPHERE;
	data->objects[0].data = sph_1;
	data->objects[1].type = SPHERE;
	data->objects[1].data = sph_2;
	data->objects[2].type = PLANE;
	data->objects[2].data = pl_1;
	data->objects[3].type = END;
}
