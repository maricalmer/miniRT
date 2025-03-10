#include "minirt.h"

static int	alloc_float_array(float ***array, int count);
static int	alloc_faces(t_obj_parser *parser);

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
	if (alloc_faces(parser) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	alloc_float_array(float ***array, int count)
{
	int		i;
	float	*block;

	*array = malloc(sizeof(float *) * count);
	if (!*array)
		return (print_error(3), EXIT_FAILURE);
	block = malloc(sizeof(float) * 3 * count);
	if (!block)
		return (free(*array), print_error(3), EXIT_FAILURE);
	i = 0;
	while (i < count)
	{
		(*array)[i] = block + (i * 3);
		i++;
	}
	return (EXIT_SUCCESS);
}


static int	alloc_faces(t_obj_parser *parser)
{
	int		i;
	int		*face_block;

	parser->faces = malloc(sizeof(int *) * parser->n_f);
	if (!parser->faces)
		return (print_error(3), EXIT_FAILURE);
	face_block = malloc(sizeof(int) * 6 * parser->n_f);
	if (!face_block)
		return (free(parser->vertices), free(parser->normals),
			free(parser->faces), print_error(3), EXIT_FAILURE);
	i = 0;
	while (i < parser->n_f)
	{
		parser->faces[i] = face_block + (i * 6);
		i++;
	}
	return (EXIT_SUCCESS);
}

