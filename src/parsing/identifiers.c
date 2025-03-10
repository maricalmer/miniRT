#include "minirt.h"

int	is_object_file(char *specs)
{
	return (specs[0] == 'o' && specs[1] == ' ');
}

int	is_light(char *specs)
{
	return (specs[0] == 'L' && specs[1] == ' ');
}

int	is_cam(char *specs)
{
	return (specs[0] == 'C' && specs[1] == ' ');
}

int	is_ambient(char *specs)
{
	return (specs[0] == 'A' && specs[1] == ' ');
}

int	is_plane(char *specs)
{
	return (specs[0] == 'p' && specs[1] == 'l' && specs[2] == ' ');
}

