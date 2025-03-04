#include "minirt.h"

int	is_right_extension(char *str, char *ext);
int	check_input_file(char *file, char *ext, int *fd);

int	check_input(int ac, char **av, t_scn *scn)
{
	if (ac != 2 && ac != 3)
	{
		print_error(1);
		return (EXIT_FAILURE);
	}
	if (ac == 2)
		return (check_input_file(av[1], ".rt", &scn->rt_fd));
	if (ac == 3)
	{
		if (check_input_file(av[1], ".rt", &scn->rt_fd) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		return (check_input_file(av[2], ".obj", &scn->obj_fd));
	}
	return (EXIT_SUCCESS);
}

int	check_input_file(char *file, char *ext, int *fd)
{
	if (!ft_strncmp(file, "", 1) || !is_right_extension(file, ext))
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

int	is_right_extension(char *str, char *ext)
{
	int	str_len;
	int	ext_len;

	str_len = ft_strlen(str);
	ext_len = ft_strlen(ext);
	if (!ft_strncmp(str + str_len - ext_len, ext, ext_len))
		return (1);
	return (0);
}

