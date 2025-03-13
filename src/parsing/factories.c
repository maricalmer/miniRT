#include "minirt.h"

int		create_ambient_light(t_data *data, char *specs);
int		create_cam(t_data *data, char *specs);
int		create_light(t_data *data, char *specs);
int		create_sphere(t_data *data, char *specs);
int		create_plane(t_data *data, char *specs);
int		create_cylinder(t_data *data, char *specs);


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

int	create_sphere(t_data *data, char *specs)
{
    if (get_coord(&specs, &data->objects[data->objects_idx].geo.sph.center[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &data->objects[data->objects_idx].geo.sph.center[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &data->objects[data->objects_idx].geo.sph.center[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_radius(&specs, &data->objects[data->objects_idx].geo.sph.radius) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[data->objects_idx].mat.rgb[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[data->objects_idx].mat.rgb[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[data->objects_idx].mat.rgb[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->objects[data->objects_idx].type = SPHERE;
	data->objects[data->objects_idx].mat.checker_flag = 0;
	// BONUS
	if (get_ratio(&specs, &data->objects[data->objects_idx].mat.refl_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &data->objects[data->objects_idx].mat.refr_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_refr_idx(&specs, &data->objects[data->objects_idx].mat.refr_idx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	create_plane(t_data *data, char *specs)
{	
    if (get_coord(&specs, &data->objects[data->objects_idx].geo.pl.point[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &data->objects[data->objects_idx].geo.pl.point[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &data->objects[data->objects_idx].geo.pl.point[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_vec_normalized(&specs, &data->objects[data->objects_idx].geo.pl.normal[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_vec_normalized(&specs, &data->objects[data->objects_idx].geo.pl.normal[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_vec_normalized(&specs, &data->objects[data->objects_idx].geo.pl.normal[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[data->objects_idx].mat.rgb[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[data->objects_idx].mat.rgb[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[data->objects_idx].mat.rgb[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->objects[data->objects_idx].type = PLANE;
	// BONUS
	if (get_ratio(&specs, &data->objects[data->objects_idx].mat.refl_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &data->objects[data->objects_idx].mat.refr_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_refr_idx(&specs, &data->objects[data->objects_idx].mat.refr_idx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_checkerboard_flag(&specs, &data->objects[data->objects_idx].mat.checker_flag) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (data->objects[data->objects_idx].mat.checker_flag)
	{
		if (get_rgb(&specs, &data->objects[data->objects_idx].mat.rgb2[0]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (get_rgb(&specs, &data->objects[data->objects_idx].mat.rgb2[1]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (get_rgb(&specs, &data->objects[data->objects_idx].mat.rgb2[2]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (get_vec_normalized(&specs, &data->objects[data->objects_idx].geo.pl.u[0]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
    	if (get_vec_normalized(&specs, &data->objects[data->objects_idx].geo.pl.u[1]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
    	if (get_vec_normalized(&specs, &data->objects[data->objects_idx].geo.pl.u[2]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (get_vec_normalized(&specs, &data->objects[data->objects_idx].geo.pl.v[0]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
    	if (get_vec_normalized(&specs, &data->objects[data->objects_idx].geo.pl.v[1]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
    	if (get_vec_normalized(&specs, &data->objects[data->objects_idx].geo.pl.v[2]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	create_cylinder(t_data *data, char *specs)
{
    if (get_coord(&specs, &data->objects[data->objects_idx].geo.cyl.center[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &data->objects[data->objects_idx].geo.cyl.center[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_coord(&specs, &data->objects[data->objects_idx].geo.cyl.center[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_vec_normalized(&specs, &data->objects[data->objects_idx].geo.cyl.dir[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_vec_normalized(&specs, &data->objects[data->objects_idx].geo.cyl.dir[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_vec_normalized(&specs, &data->objects[data->objects_idx].geo.cyl.dir[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_radius(&specs, &data->objects[data->objects_idx].geo.cyl.radius) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    if (get_length(&specs, &data->objects[data->objects_idx].geo.cyl.height) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[data->objects_idx].mat.rgb[0]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[data->objects_idx].mat.rgb[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, &data->objects[data->objects_idx].mat.rgb[2]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->objects[data->objects_idx].type = CYLINDER;
    data->objects[data->objects_idx].mat.checker_flag = 0;
	// BONUS
	if (get_ratio(&specs, &data->objects[data->objects_idx].mat.refl_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &data->objects[data->objects_idx].mat.refr_coeff) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_refr_idx(&specs, &data->objects[data->objects_idx].mat.refr_idx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}





int create_triangle(t_data *data, char *line, t_obj_parser *parser)
{
    int	v[3];
	int	vn[3];
	int vt[3];
	int	i;

	if ((sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
		&v[0], &vt[0], &vn[0], 
		&v[1], &vt[1], &vn[1], 
		&v[2], &vt[2], &vn[2]) == 9) ||  (sscanf(line, "f %d//%d %d//%d %d//%d", 
		&v[0], &vn[0], 
		&v[1], &vn[1], 
		&v[2], &vn[2]) == 6))
	{
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
			data->objects[data->objects_idx].geo.tri.v0[i] = parser->vertices[v[0]][i];
			data->objects[data->objects_idx].geo.tri.v1[i] = parser->vertices[v[1]][i];
			data->objects[data->objects_idx].geo.tri.v2[i] = parser->vertices[v[2]][i];
	
			data->objects[data->objects_idx].geo.tri.n0[i] = parser->normals[vn[0]][i];
			data->objects[data->objects_idx].geo.tri.n1[i] = parser->normals[vn[1]][i];
			data->objects[data->objects_idx].geo.tri.n2[i] = parser->normals[vn[2]][i];
			i++;
		}

		normalize(data->objects[data->objects_idx].geo.tri.n0);
		normalize(data->objects[data->objects_idx].geo.tri.n1);
		normalize(data->objects[data->objects_idx].geo.tri.n2);

		data->objects[data->objects_idx].type = TRI;
		data->objects[data->objects_idx].mat.checker_flag = 0;
		data->objects[data->objects_idx].mat.refl_coeff = parser->tri_refl_coeff;
		data->objects[data->objects_idx].mat.refr_coeff = parser->tri_refr_coeff;
		data->objects[data->objects_idx].mat.refr_idx = parser->tri_refr_idx;
		if (parser->tri_rgb[0] == -1)
		{
			data->objects[data->objects_idx].mat.rgb[0] = rand() % 256;
			data->objects[data->objects_idx].mat.rgb[1] = rand() % 256;
			data->objects[data->objects_idx].mat.rgb[2] = rand() % 256;
		}
		else
		{
			data->objects[data->objects_idx].mat.rgb[0] = parser->tri_rgb[0];
			data->objects[data->objects_idx].mat.rgb[1] = parser->tri_rgb[1];
			data->objects[data->objects_idx].mat.rgb[2] = parser->tri_rgb[2];
		}
		data->objects_idx++;
		return (EXIT_SUCCESS);
	}
	else
		return (EXIT_FAILURE);
}
