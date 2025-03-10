#include "minirt.h"

int	is_between_0_and_1(double n);
int	is_between_neg1_and_1(double n);
int	is_between_0_and_180(int n);
int	is_between_0_and_255(int n);
int	is_between_0_and_2_7(double n);
int	is_0_or_1(int n);

__attribute__((optimize("O0"))) int	get_ratio(char **specs, float *ratio)
{
	if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	*ratio = ft_strtof(*specs, specs);
	if (errno == ERANGE || !is_between_0_and_1(*ratio))
		return (EXIT_FAILURE);
	if (**specs != ' ')
		return (EXIT_FAILURE);
	(*specs)++;
	return (EXIT_SUCCESS);
}

__attribute__((optimize("O0"))) int	get_rgb_normalized(char **specs, float *color)
{
	if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	*color = ft_strtof(*specs, specs) / 255.0f;
	if (errno == ERANGE || !is_between_0_and_1(*color))
		return (EXIT_FAILURE);
	while (!ft_isdigit(**specs) && **specs != '\0')
	{
		if (**specs != ' ' && **specs != ',')
			return (EXIT_FAILURE);
		(*specs)++;
	}
	return (EXIT_SUCCESS);
}

__attribute__((optimize("O0"))) unsigned char	get_rgb(char **specs, unsigned char *color)
{
	if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	*color = ft_strtoi(*specs, specs);
	if (errno == ERANGE || !is_between_0_and_255(*color))
		return (EXIT_FAILURE);
	while (!ft_isdigit(**specs) && **specs != '\0')
	{
		if (**specs != ' ' && **specs != ',')
			return (EXIT_FAILURE);
		(*specs)++;
	}
	return (EXIT_SUCCESS);
}

__attribute__((optimize("O0"))) int	get_refr_idx(char **specs, float *ratio)
{
	char *p;
	if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	p = *specs;
	*ratio = ft_strtof(*specs, specs);
	if (errno == ERANGE || !is_between_0_and_2_7(*ratio))
		return (EXIT_FAILURE);
	if (**specs != ' ' && *specs != p)
		return (EXIT_FAILURE);
	if (**specs == ' ')
		(*specs)++;
	return (EXIT_SUCCESS);
}

__attribute__((optimize("O0"))) int	get_coord(char **specs, float *value)
{
	if (!ft_isdigit(**specs) && **specs != '-')
		return (EXIT_FAILURE);
	*value = ft_strtof(*specs, specs);
	if (errno == ERANGE)
		return (EXIT_FAILURE);
	while (!ft_isdigit(**specs) && **specs != '\0' && **specs != '-')
	{
		if (**specs != ' ' && **specs != ',')
			return (EXIT_FAILURE);
		(*specs)++;
	}
	return (EXIT_SUCCESS);
}

__attribute__((optimize("O0"))) int get_vec_normalized(char **specs, float *value)
{
    if (!ft_isdigit(**specs) && **specs != '-')
		return (EXIT_FAILURE);
	*value = ft_strtof(*specs, specs);
	if (errno == ERANGE || !is_between_neg1_and_1(*value))
		return (EXIT_FAILURE);
	while (!ft_isdigit(**specs) && **specs != '\0' && **specs != '-')
	{
		if (**specs != ' ' && **specs != ',')
			return (EXIT_FAILURE);
		(*specs)++;
	}
	return (EXIT_SUCCESS);
}

__attribute__((optimize("O0"))) int get_radius(char **specs, float *radius)
{
    if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	*radius = ft_strtof(*specs, specs) * 0.5f;
	if (errno == ERANGE)
		return (EXIT_FAILURE);
	if (**specs != ' ')
		return (EXIT_FAILURE);
	(*specs)++;
	return (EXIT_SUCCESS);
}

__attribute__((optimize("O0"))) int get_length(char **specs, float *length)
{
    if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	*length = ft_strtof(*specs, specs);
	if (errno == ERANGE)
		return (EXIT_FAILURE);
	if (**specs != ' ')
		return (EXIT_FAILURE);
	(*specs)++;
	return (EXIT_SUCCESS);
}

__attribute__((optimize("O0"))) int get_fov_range(char **specs, int *fov)
{
    if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	*fov = ft_strtoi(*specs, specs);
	if (errno == ERANGE || !is_between_0_and_180(*fov))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

__attribute__((optimize("O0"))) int get_checkerboard_flag(char **specs, int *flag)
{
    if (!ft_isdigit(**specs))
		return (EXIT_FAILURE);
	*flag = ft_strtoi(*specs, specs);
	if (errno == ERANGE || !is_0_or_1(*flag))
		return (EXIT_FAILURE);
	if (**specs != ' ')
		return (EXIT_FAILURE);
	(*specs)++;
	return (EXIT_SUCCESS);
}

int	is_between_neg1_and_1(double n)
{
	return (n >= -1.0 && n <= 1.0);
}

int	is_between_0_and_1(double n)
{
	return (n >= 0.0 && n <= 1.0);
}

int	is_0_or_1(int n)
{
	return (n == 0 || n == 1);
}

int	is_between_0_and_2_7(double n)
{
	return (n >= 0.0 && n <= 2.7);
}

int	is_between_0_and_180(int n)
{
	return (n >= 0 && n <= 180);
}

int	is_between_0_and_255(int n)
{
	return (n >= 0 && n <= 255);
}
