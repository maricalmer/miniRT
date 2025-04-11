/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_factories4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 23:08:40 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/11 15:35:21 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	test_create_light_success(void)
{
	t_data data;
	data.objects_idx = 0;
	char input[] = "0,1000,1000 0.6 255,255,255";
	char *specs = input;
	data.lights = malloc(sizeof(t_light) * 1);

	CU_ASSERT_PTR_NOT_NULL_FATAL(data.lights);
	int result = create_light(&data, specs);
	CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
	CU_ASSERT_TRUE(assert_float_equal(data.lights[0].origin[0], 0.0f, EPSILON_TEST));
	printf("\n----> data.lights[0].brightness: %f\n", data.lights[0].brightness);
	CU_ASSERT_TRUE(assert_float_equal(data.lights[0].brightness, 0.6f, EPSILON_TEST));
	printf("\n----> data.lights[0].rgb[0]: %f\n", data.lights[0].rgb[0]);
	CU_ASSERT_TRUE(assert_float_equal(data.lights[0].rgb[0], 1.0f, EPSILON_TEST));
	free(data.lights);
}

int	add_factories4_tests(void)
{
	CU_pSuite	suite;

	suite = CU_add_suite("factories4", NULL, NULL);
	if (!suite)
		return (CU_get_error());
	if (!CU_add_test(suite, "test_create_light_success", test_create_light_success))
		return (CU_get_error());
	return (CUE_SUCCESS);
}

