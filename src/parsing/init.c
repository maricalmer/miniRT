#include "minirt.h"

static int	alloc_float_array(float (**array)[3], int count);

int	init_rt_lists(t_data *data)
{
	data->objects = malloc(sizeof(t_object) * data->n_obj);
	if (!data->objects)
		return (print_error(3), EXIT_FAILURE);
	data->lights = malloc(sizeof(t_light) * data->n_light);
	if (!data->lights)
		return (free(data->objects), print_error(3), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	init_obj_lists(t_obj_parser *parser)
{
	if (alloc_float_array(&parser->vertices, parser->n_v) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (alloc_float_array(&parser->normals, parser->n_vn) == EXIT_FAILURE)
		return (free(parser->vertices), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}


static int	alloc_float_array(float (**array)[3], int count)
{
	*array = malloc(sizeof(float [3]) * count);
	if (!*array)
		return (print_error(3), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

