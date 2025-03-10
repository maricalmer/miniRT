#include "minirt.h"

int		create_ambient_light(t_data *data, char *specs);
int		create_cam(t_data *data, char *specs);
int		create_light(t_data *data, char *specs);
int		create_sphere(t_data *data, char *specs, int index);
int		create_plane(t_data *data, char *specs, int index);
int		create_cylinder(t_data *data, char *specs, int index);


int	create_ambient_light(t_data *data, char *specs)
{
	t_ambient_light	ambient;

	if (get_ratio(&specs, &ambient.brightness) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb_normalized(&specs, &ambient.rgb[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb_normalized(&specs, &ambient.rgb[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb_normalized(&specs, &ambient.rgb[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->ambient = ambient;
	return (EXIT_SUCCESS);
}


int	create_cam(t_data *data, char *specs)
{
	t_camera	camera;

	if (get_coord(&specs, &camera.origin[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &camera.origin[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &camera.origin[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_vec_normalized(&specs, &camera.direction[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_vec_normalized(&specs, &camera.direction[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_vec_normalized(&specs, &camera.direction[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	normalize(camera.direction);
	if (get_fov_range(&specs, &camera.fov) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->cam = camera;
	return (EXIT_SUCCESS);
}

int	create_light(t_data *data, char *specs)
{
	t_light	light;
    static int     i;

	if (get_coord(&specs, &light.origin[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &light.origin[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &light.origin[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &light.brightness) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb_normalized(&specs, &light.rgb[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb_normalized(&specs, &light.rgb[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb_normalized(&specs, &light.rgb[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	// BONUS
	data->lights[i++] = light;
	return (EXIT_SUCCESS);
}

int	create_sphere(t_data *data, char *specs, int index)
{
    if (get_coord(&specs, &data->objects[index].geo.sph.center[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &data->objects[index].geo.sph.center[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &data->objects[index].geo.sph.center[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_radius(&specs, &data->objects[index].geo.sph.radius) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[index].mat.rgb[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[index].mat.rgb[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[index].mat.rgb[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->objects[index].type = SPHERE;
	data->objects[index].mat.checker_flag = 0;
	// BONUS
	if (get_ratio(&specs, &data->objects[index].mat.refl_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &data->objects[index].mat.refr_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_refr_idx(&specs, &data->objects[index].mat.refr_idx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	create_plane(t_data *data, char *specs, int index)
{	
    if (get_coord(&specs, &data->objects[index].geo.pl.point[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &data->objects[index].geo.pl.point[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &data->objects[index].geo.pl.point[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_vec_normalized(&specs, &data->objects[index].geo.pl.normal[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_vec_normalized(&specs, &data->objects[index].geo.pl.normal[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_vec_normalized(&specs, &data->objects[index].geo.pl.normal[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[index].mat.rgb[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[index].mat.rgb[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[index].mat.rgb[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->objects[index].type = PLANE;
	// BONUS
	if (get_ratio(&specs, &data->objects[index].mat.refl_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &data->objects[index].mat.refr_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_refr_idx(&specs, &data->objects[index].mat.refr_idx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_checkerboard_flag(&specs, &data->objects[index].mat.checker_flag) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (data->objects[index].mat.checker_flag)
	{
		if (get_rgb(&specs, &data->objects[index].mat.rgb2[0]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (get_rgb(&specs, &data->objects[index].mat.rgb2[1]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (get_rgb(&specs, &data->objects[index].mat.rgb2[2]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (get_vec_normalized(&specs, &data->objects[index].geo.pl.u[0]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
    	if (get_vec_normalized(&specs, &data->objects[index].geo.pl.u[1]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
    	if (get_vec_normalized(&specs, &data->objects[index].geo.pl.u[2]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (get_vec_normalized(&specs, &data->objects[index].geo.pl.v[0]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
    	if (get_vec_normalized(&specs, &data->objects[index].geo.pl.v[1]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
    	if (get_vec_normalized(&specs, &data->objects[index].geo.pl.v[2]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	create_cylinder(t_data *data, char *specs, int index)
{
    if (get_coord(&specs, &data->objects[index].geo.cyl.center[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &data->objects[index].geo.cyl.center[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &data->objects[index].geo.cyl.center[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_vec_normalized(&specs, &data->objects[index].geo.cyl.dir[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_vec_normalized(&specs, &data->objects[index].geo.cyl.dir[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_vec_normalized(&specs, &data->objects[index].geo.cyl.dir[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_radius(&specs, &data->objects[index].geo.cyl.radius) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_length(&specs, &data->objects[index].geo.cyl.height) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[index].mat.rgb[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[index].mat.rgb[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[index].mat.rgb[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->objects[index].type = CYLINDER;
    data->objects[index].mat.checker_flag = 0;
	// BONUS
	if (get_ratio(&specs, &data->objects[index].mat.refl_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &data->objects[index].mat.refr_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_refr_idx(&specs, &data->objects[index].mat.refr_idx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}



int create_triangle(t_data *data, char *line, t_obj_parser *parser, int *idx)
{
    int	v[3];
	int	vn[3];
	int dummy[3];
	int	i;

    if (sscanf(line, "f %d//%d %d//%d %d//%d",
           &v[0], &vn[0], &v[1], &vn[1], &v[2], &vn[2]) != 6)
    // if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
	// 	&v[0], &dummy[0], &vn[0], &v[1], &dummy[1], &vn[1], &v[2], &dummy[2], &vn[2]) != 9)
		return (EXIT_FAILURE);
    i = 0;
	while (i < 3)
    {
        v[i]--;
        vn[i]--;
		i++;
    }
	i = 0;
	while (i <  3)
    {
		data->objects[*idx].geo.tri.v0[i] = parser->vertices[v[0]][i];
		data->objects[*idx].geo.tri.v1[i] = parser->vertices[v[1]][i];
		data->objects[*idx].geo.tri.v2[i] = parser->vertices[v[2]][i];

		data->objects[*idx].geo.tri.n0[i] = parser->normals[vn[0]][i];
		data->objects[*idx].geo.tri.n1[i] = parser->normals[vn[1]][i];
		data->objects[*idx].geo.tri.n2[i] = parser->normals[vn[2]][i];
		i++;
	}
	normalize(data->objects[*idx].geo.tri.n0);
	normalize(data->objects[*idx].geo.tri.n1);
	normalize(data->objects[*idx].geo.tri.n2);
	
	data->objects[*idx].type = TRI;
    data->objects[*idx].mat.checker_flag = 0;
	data->objects[*idx].mat.refl_coeff = 0;
	data->objects[*idx].mat.refr_coeff = 0.95;
	data->objects[*idx].mat.refr_idx = 1.5;
	data->objects[*idx].mat.rgb[0] = 200;
	data->objects[*idx].mat.rgb[1] = 200;
	data->objects[*idx].mat.rgb[2] = 200;
	(*idx)++;
    //t->mesh_id = 0;
	return (EXIT_SUCCESS);
}
