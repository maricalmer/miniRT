#include "minirt.h"

static int	alloc_float_array(float (**array)[3], int count);

void	init_parsers(t_obj_parser *parsers, int n_parsers)
{
	int	i;

	i = 0;
	while (i < n_parsers)
	{
		parsers[i].n_f = 0;
		parsers[i].n_vn = 0;
		parsers[i].n_v = 0;
		parsers[i].idx_n = 0;
		parsers[i].idx_v = 0;
		i++;
	}
}

int	init_elem_rt(t_data *data)
{
	data->objects = malloc(sizeof(t_object) * data->n_obj);
	if (!data->objects)
		return (print_error(3), EXIT_FAILURE);
	data->lights = malloc(sizeof(t_light) * data->n_light);
	if (!data->lights)
		return (print_error(3), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	init_elem_obj(t_obj_parser *parser)
{
	if (alloc_float_array(&parser->vertices, parser->n_v) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (alloc_float_array(&parser->normals, parser->n_vn) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}


// static int	alloc_float_array(float ***array, int count)
// {
// 	int		i;
// 	float	*block;

// 	*array = malloc(sizeof(float *) * count);
// 	if (!*array)
// 		return (print_error(3), EXIT_FAILURE);
// 	block = malloc(sizeof(float) * 3 * count);
// 	if (!block)
// 		return (free(*array), print_error(3), EXIT_FAILURE);
// 	i = 0;
// 	while (i < count)
// 	{
// 		(*array)[i] = block + (i * 3);
// 		i++;
// 	}
// 	return (EXIT_SUCCESS);
// }

static int	alloc_float_array(float (**array)[3], int count)
{
	*array = malloc(sizeof(float [3]) * count);
	if (!*array)
		return (print_error(3), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

