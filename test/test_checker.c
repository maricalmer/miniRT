/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_checker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 19:46:09 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/05 21:19:11 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	mock_open_success(const char *path, int flags)
{
	(void)path;
	(void)flags;
	return (42);
}

int	mock_open_fail(const char *path, int flags)
{
	(void)path;
	(void)flags;
	return (-1);
}

int (*test_open)(const char *, int) = mock_open_success;
#define open(path, flags) test_open(path, flags)

int	is_rt_extension(char *str)
{
	int	len = ft_strlen(str);

	if (len < 4)
		return (0);
	return (ft_strncmp(str + len - 3, ".rt", 3) == 0);
}

int	check_filename_test(char *file, int *fd)
{
	if (!ft_strncmp(file, "", 1) || !is_rt_extension(file))
		return (EXIT_FAILURE);
	*fd = open(file, O_RDONLY);
	if (*fd < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	check_input_test(int ac, char **av, t_data *data)
{
	if (ac != 2)
		return (EXIT_FAILURE);
	if (check_filename_test(av[1], &data->rt_fd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	test_check_input_argc_invalid(void)
{
	t_data	dummy;
	char *argv[] = {"pgm"};
	int	result = check_input_test(1, argv, &dummy);

	CU_ASSERT_EQUAL(result, EXIT_FAILURE);
}

void	test_check_input_empty_filename(void)
{
	t_data dummy;
	char *argv[] = {"pgm", ""};
	int result = check_input_test(2, argv, &dummy);

	CU_ASSERT_EQUAL(result, EXIT_FAILURE);
}

void	test_check_input_invalid_extension(void)
{
	t_data dummy;
	char *argv[] = {"pgm", "scene.txt"};
	int result = check_input_test(2, argv, &dummy);

	CU_ASSERT_EQUAL(result, EXIT_FAILURE);
}

void	test_check_input_file_open_fail(void)
{
	t_data dummy;
	char *argv[] = {"pgm", "file.rt"};
	test_open = mock_open_fail;
	int result = check_input_test(2, argv, &dummy);

	CU_ASSERT_EQUAL(result, EXIT_FAILURE);
}

void	test_check_input_success(void)
{
	t_data dummy;
	char *argv[] = {"pgm", "file.rt"};
	test_open = mock_open_success;
	int result = check_input_test(2, argv, &dummy);

	CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
}

int	add_checker_tests(void)
{
	CU_pSuite	suite;

	suite = CU_add_suite("checker", NULL, NULL);
	if (!suite)
		return (CU_get_error());
	if (!CU_add_test(suite, "test_check_input_argc_invalid", test_check_input_argc_invalid)
		|| !CU_add_test(suite, "test_check_input_empty_filename", test_check_input_empty_filename)
		|| !CU_add_test(suite, "test_check_input_invalid_extension", test_check_input_invalid_extension)
		|| !CU_add_test(suite, "test_check_input_file_open_fail", test_check_input_file_open_fail)
		|| !CU_add_test(suite, "test_check_input_success", test_check_input_success))
	{
		return (CU_get_error());
	}

	return (CUE_SUCCESS);
}