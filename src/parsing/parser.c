#include "minirt.h"

static void	apply_uniform_spacing(char *specs, char *start, char *end);

int	handle_parsing(char **av, t_data *data)
{
	if (read_and_count_data_in_rt(data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (data->n_obj_files)
	{
		if (parse_obj_files(data, av[1]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else
	{
		if (init_elem_rt(data) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (create_elements_rt(data, av[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

char	*format_string(char *str, int len)
{
	char	*specs;
	int		i;
	int		size;
	char	*start;
	char	*end;

	i = 0;
	while (ft_iswhitespace(str[i]))
		i++;
	start = &str[i];
	while (len > 0 && ft_iswhitespace(str[len - 1]))
		len--;
	end = &str[len - 1];
	size = end - start + 1;
	specs = malloc(sizeof(char) * (size + 1));
	if (!specs)
	{
		free(str);
		return (NULL);
	}
	apply_uniform_spacing(specs, start, end);
	free(str);
	return (specs);
}

static void	apply_uniform_spacing(char *specs, char *start, char *end)
{
	int	i;
	int	j;
	int	space_flag;

	i = 0;
	j = 0;
	space_flag = 0;
	while (start[i] && &start[i] <= end)
	{
		if (ft_iswhitespace(start[i]))
		{
			if (!space_flag)
			{
				specs[j++] = ' ';
				space_flag = 1;
			}
		}
		else
		{
			specs[j++] = start[i];
			space_flag = 0;
		}
		i++;
	}
	specs[j] = '\0';
}
