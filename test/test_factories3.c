/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_factories3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:45:09 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/06 18:51:27 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	test_create_cylinder_success(void)
{
	t_data data;
	data.objects_idx = 0;
	char input[] = "150.0,10.0,20.6 0,1,0 14.2 21.42 10,0,255 0 0.98 1.5";
	char *specs = input;
	data.objects = malloc(sizeof(t_object) * 1);

	CU_ASSERT_PTR_NOT_NULL_FATAL(data.objects);

	int result = create_cylinder(&data, specs);

	CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].geo.cyl.center[1], 10.0f, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].geo.cyl.radius, 7.1f, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].geo.cyl.height, 21.42f, EPSILON_TEST));
	free(data.objects);
}

void	test_create_rectangle_success(void)
{
	t_data data;
	data.objects_idx = 0;
	char input[] = "0,0,0 0,1.0,0 234,196,53 0 0 0 50 10 173,52,62 1,0,0 0,0,1";
	char *specs = input;
	data.objects = malloc(sizeof(t_object) * 1);

	CU_ASSERT_PTR_NOT_NULL_FATAL(data.objects);

	int result = create_rectangle(&data, specs);
	CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].geo.pl.point[1], 0.0f, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].mat.refl_coeff, 0.0f, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].mat.refr_idx, 0.0f, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].mat.checker_size, 0.10f, EPSILON_TEST));
	free(data.objects);
}

int	add_factories3_tests(void)
{
	CU_pSuite	suite;

	suite = CU_add_suite("factories3", NULL, NULL);
	if (!suite)
		return (CU_get_error());
	if (!CU_add_test(suite, "test_create_cylinder_success", test_create_cylinder_success)
		|| !CU_add_test(suite, "test_create_rectangle_success", test_create_rectangle_success))
	{
		return (CU_get_error());
	}

	return (CUE_SUCCESS);
}
