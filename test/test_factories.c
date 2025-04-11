/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_factories.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 21:23:54 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/11 17:35:32 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	test_create_ambient_light_success(void)
{
	t_data data;
	char input[] = "0.2 255,255,255";
	char *specs = input;

	int result = create_ambient_light(&data, specs);

	CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
	CU_ASSERT_TRUE(assert_float_equal(data.ambient.brightness, 0.2f, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(data.ambient.rgb[0], 1.0f, EPSILON_TEST));
}

void	test_create_cam_success(void)
{
	t_data data;
	char input[] = "0,0,0 0,0,1 60";
	char *specs = input;

	int result = create_cam(&data, specs);

	CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
	CU_ASSERT_TRUE(assert_float_equal(data.cam.origin[1], 0.0f, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(data.cam.direction[1], 0.0f, EPSILON_TEST));
	CU_ASSERT_EQUAL(data.cam.fov, 60);
}

void	test_create_plane_success(void)
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

int	add_factories_tests(void)
{
	CU_pSuite	suite;

	suite = CU_add_suite("factories", NULL, NULL);
	if (!suite)
		return (CU_get_error());
	if (!CU_add_test(suite, "test_create_ambient_light_success", test_create_ambient_light_success)
		|| !CU_add_test(suite, "test_create_cam_success", test_create_cam_success)
		|| !CU_add_test(suite, "test_create_plane_success", test_create_plane_success))
	{
		return (CU_get_error());
	}

	return (CUE_SUCCESS);
}
