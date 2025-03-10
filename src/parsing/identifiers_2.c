#include "minirt.h"

int	is_sphere(char *specs)
{
	return (specs[0] == 's' && specs[1] == 'p' && specs[2] == ' ');
}

int	is_cylinder(char *specs)
{
	return (specs[0] == 'c' && specs[1] == 'y' && specs[2] == ' ');
}

int	is_face(char *specs)
{
	return (specs[0] == 'f' && specs[1] == ' ');
}

int	is_smoothing(char *specs)
{
	return (specs[0] == 's' && specs[1] == ' ');
}

int	increase_if_uniq(int *value)
{
	if (!*value)
		(*value)++;
	else
		return (print_error(4), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
