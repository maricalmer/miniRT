/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_runner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:01:29 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/11 15:52:44 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	main(void)
{
	int	failed;

	setlocale(LC_NUMERIC, "C");
	if (CU_initialize_registry() != CUE_SUCCESS)
		return (CU_get_error());
	if (add_dot_in_place_33_tests() != CUE_SUCCESS
		|| add_dot_in_place_44_tests() != CUE_SUCCESS
		|| add_math_tests() != CUE_SUCCESS
		|| add_find_median_tests() != CUE_SUCCESS
		|| add_rodrigues_tests() != CUE_SUCCESS
		|| add_checker_tests() != CUE_SUCCESS
		|| add_factories_tests() != CUE_SUCCESS
		|| add_factories2_tests() != CUE_SUCCESS
		|| add_factories3_tests() != CUE_SUCCESS
		|| add_factories4_tests() != CUE_SUCCESS
		|| add_sphere_tests() != CUE_SUCCESS
		|| add_rectangle_tests() != CUE_SUCCESS
		|| add_plane_tests() != CUE_SUCCESS
		|| add_cylinder_tests() != CUE_SUCCESS
		|| add_triangle_tests() != CUE_SUCCESS)
	{
		CU_cleanup_registry();
		return (CU_get_error());
	}
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	failed = CU_get_number_of_failures();
	CU_cleanup_registry();
	if (failed > 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
