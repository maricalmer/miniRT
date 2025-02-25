#include <minirt.h>

int		create_ambient_light(t_data *data, char *specs);
int		create_cam(t_data *data, char *specs);
int		create_light(t_data *data, char *specs);
int		create_sphere(t_data *data, char *specs);
int		create_plane(t_data *data, char *specs);
int		create_cylinder(t_data *data, char *specs);


int	create_ambient_light(t_data *data, char *specs)
{
	t_ambient_light	ambient;

	if (!ft_isdigit(*specs))
		return (EXIT_FAILURE);
	while (*specs)
	{
		ambient.brightness = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (!ft_isdigit(*specs))
			return (EXIT_FAILURE);
		ambient.rgb[0] = ft_strtof(specs, &specs) / 255.0f;
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (!ft_isdigit(*specs))
			return (EXIT_FAILURE);
		ambient.rgb[1] = ft_strtof(specs, &specs) / 255.0f;
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (!ft_isdigit(*specs))
			return (EXIT_FAILURE);
		ambient.rgb[2] = ft_strtof(specs, &specs) / 255.0f;
	}
	printf("HERE??\n");
	data->ambient = ambient;
}

int	create_cam(t_data *data, char *specs)
{
	t_camera	camera;

	if (*specs != '-' && !ft_isdigit(*specs))
		return (EXIT_FAILURE);
	while (*specs)
	{
		camera.origin[0] = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (*specs != '-' && !ft_isdigit(*specs))
			return (EXIT_FAILURE);
		camera.origin[1] = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (*specs != '-' && !ft_isdigit(*specs))
			return (EXIT_FAILURE);
		camera.origin[2] = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (*specs != '-' && !ft_isdigit(*specs))
			return (EXIT_FAILURE);
		camera.direction[0] = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (*specs != '-' && !ft_isdigit(*specs))
			return (EXIT_FAILURE);
		camera.direction[1] = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (*specs != '-' && !ft_isdigit(*specs))
			return (EXIT_FAILURE);
		camera.direction[2] = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (!ft_isdigit(*specs))
			return (EXIT_FAILURE);
		camera.fov = (int)ft_strtof(specs, &specs);
	}
	data->cam = camera;
}

int	create_light(t_data *data, char *specs)
{
	t_light	light;
	int		i;

	if (*specs != '-' && !ft_isdigit(*specs))
		return (EXIT_FAILURE);
	while (*specs)
	{
		light.origin[0] = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (*specs != '-' && !ft_isdigit(*specs))
			return (EXIT_FAILURE);
		light.origin[1] = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (*specs != '-' && !ft_isdigit(*specs))
			return (EXIT_FAILURE);
		light.origin[2] = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (!ft_isdigit(*specs))
			return (EXIT_FAILURE);
		light.brightness = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (!ft_isdigit(*specs))
			return (EXIT_FAILURE);
		light.rgb[0] = ft_strtof(specs, &specs) / 255.0f;
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (!ft_isdigit(*specs))
			return (EXIT_FAILURE);
		light.rgb[1] = ft_strtof(specs, &specs) / 255.0f;
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (!ft_isdigit(*specs))
			return (EXIT_FAILURE);
		light.rgb[2] = ft_strtof(specs, &specs) / 255.0f;
	}
	i = 0;
	while (data->lights[i].rgb[0] != -1) // dirty check
		i++;
	data->lights[i] = light;
}

int	create_sphere(t_data *data, char *specs)
{
	t_sphere	sphere;
	int			i;

	if (*specs != '-' && !ft_isdigit(*specs))
		return (EXIT_FAILURE);
	while (*specs)
	{
		sphere.center[0] = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (*specs != '-' && !ft_isdigit(*specs))
			return (EXIT_FAILURE);
		sphere.center[1] = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (*specs != '-' && !ft_isdigit(*specs))
			return (EXIT_FAILURE);
		sphere.center[2] = ft_strtof(specs, &specs);
		while (ft_iswhitespace(*specs) || *specs == ',')
			specs++;
		if (!ft_isdigit(*specs))
			return (EXIT_FAILURE);
		sphere.radius = ft_strtof(specs, &specs) / 2.0f;
		// while (ft_iswhitespace(*specs) || *specs == ',')
		// 	specs++;
		// if (!ft_isdigit(*specs))
		// 	return (EXIT_FAILURE);
		// sphere.rgb[0] = ft_strtof(specs, &specs) / 255.0f;
		// while (ft_iswhitespace(*specs) || *specs == ',')
		// 	specs++;
		// if (!ft_isdigit(*specs))
		// 	return (EXIT_FAILURE);
		// sphere.rgb[1] = ft_strtof(specs, &specs) / 255.0f;
		// while (ft_iswhitespace(*specs) || *specs == ',')
		// 	specs++;
		// if (!ft_isdigit(*specs))
		// 	return (EXIT_FAILURE);
		// sphere.rgb[2] = ft_strtof(specs, &specs) / 255.0f;
	}
	i = 0;
	while (data->objects[i].type != EMPTY) // dirty check
		i++;
	
}

int	create_plane(t_data *data, char *specs)
{
	(void) data;
	printf("create plane\n");
}

int	create_cylinder(t_data *data, char *specs)
{
	(void) data;
	printf("create cylinder\n");
}
