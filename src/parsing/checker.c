#include <minirt.h>

int	is_rt(char *str);

int	check_input(int ac, char **av, t_scn *scn)
{
	if (ac != 2)
	{
		print_error(1);
		return (EXIT_FAILURE);
	}
	if (!ft_strncmp(av[1], "", 1) || !is_rt(av[1]))
	{
		print_error(2);
		return (EXIT_FAILURE);
	}
	scn->fd = open(av[1], O_RDONLY);
	if (scn->fd < 0)
	{
		print_error(3);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	is_rt(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (!ft_strncmp(str + len - 3, ".rt", 3))
		return (1);
	return (0);
}

