#include "minirt.h"

static int	is_rt_extension(char *str);
static int	check_input_file(char *file, int *fd);

int	check_input(int ac, char **av, t_data *data)
{
	if (ac != 2)
	{
		print_error(1);
		return (EXIT_FAILURE);
	}
	if (check_input_file(av[1], &data->rt_fd) == EXIT_FAILURE) 
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	check_input_file(char *file, int *fd)
{
	if (!ft_strncmp(file, "", 1) || !is_rt_extension(file))
	{
		print_error(2);
		return (EXIT_FAILURE);
	}
	*fd = open(file, O_RDONLY);
	if (*fd < 0)
	{
		print_error(3);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	is_rt_extension(char *str)
{
	int	str_len;

	str_len = ft_strlen(str);
	if (str_len < 4)
		return (0);
	if (!ft_strncmp(str + str_len - 3, ".rt", 3))
		return (1);
	return (0);
}

