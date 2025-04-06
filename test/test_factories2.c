/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_factories2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:24:24 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/06 18:51:21 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	test_create_sphere_success(void)
{
	t_data data;
	data.objects_idx = 0;
	char input[] = "-263.42,2.48,-650.53 4.96 255,255,0 0.10 0.23 1.01";
	char *specs = input;
	data.objects = malloc(sizeof(t_object) * 1);

	CU_ASSERT_PTR_NOT_NULL_FATAL(data.objects);

	int result = create_sphere(&data, specs);
	CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].geo.sph.center[1], 2.48f, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].geo.sph.radius, 2.48f, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].mat.refr_idx, 1.01f, EPSILON_TEST));
	free(data.objects);
}

void	test_create_triangle_success(void)
{
	t_data data;
	data.objects_idx = 0;
	char input[] = "0,0,0 0,1.0,0 2,24,39 0.25 0 0 20 5,48,78 1,0,0 0,0,1";
	char *specs = input;
	data.objects = malloc(sizeof(t_object) * 1);

	CU_ASSERT_PTR_NOT_NULL_FATAL(data.objects);

	int result = create_plane(&data, specs);
	CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
	CU_ASSERT_EQUAL(data.objects[0].type, PLANE);
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].geo.pl.normal[1], 1.0f, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].mat.refl_coeff, 0.25f, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].mat.refr_idx, 0.0f, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(data.objects[0].mat.checker_size, 0.05f, EPSILON_TEST));
	free(data.objects);
}

int	add_factories2_tests(void)
{
	CU_pSuite	suite;

	suite = CU_add_suite("factories2", NULL, NULL);
	if (!suite)
		return (CU_get_error());
	if (!CU_add_test(suite, "test_create_sphere_success", test_create_sphere_success)
		|| !CU_add_test(suite, "test_create_triangle_success", test_create_triangle_success))
	{
		return (CU_get_error());
	}

	return (CUE_SUCCESS);
}
