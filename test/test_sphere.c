/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_sphere.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:29:00 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/06 19:36:20 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static t_object	create_dummy_sphere(float center[3], float radius)
{
	t_object	obj;
	int			i;

	obj.geo.sph.radius = radius;
	i = -1;
	while (++i < 3)
		obj.geo.sph.center[i] = center[i];
	return (obj);
}

void	test_sphere_hit_center(void)
{
	float ray[3] = {0.0f, 0.0f, 1.0f};
	float origin[3] = {0.0f, 0.0f, 0.0f};
	t_object sphere = create_dummy_sphere((float[3]){0.0f, 0.0f, 5.0f}, 1.0f);
	float t = test_sphere(&sphere, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 4.0f, EPSILON_TEST));
}

void	test_sphere_from_inside(void)
{
	t_object sphere = create_dummy_sphere((float[3]){0.0f, 0.0f, 0.0f}, 2.0f);
	float ray[3] = {1.0f, 0.0f, 0.0f};
	float origin[3] = {0.0f, 0.0f, 0.0f};
	float t = test_sphere(&sphere, ray, origin);

	CU_ASSERT(t > 0.0f);
}

void	test_sphere_miss(void)
{
	t_object sphere = create_dummy_sphere((float[3]){0.0f, 0.0f, 5.0f}, 1.0f);
	float ray[3] = {0.0f, 1.0f, 0.0f};
	float origin[3] = {0.0f, 0.0f, 0.0f};
	float t = test_sphere(&sphere, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 0.0f, EPSILON_TEST));
}

void	test_sphere_tangent(void)
{
	t_object sphere = create_dummy_sphere((float[3]){1.0f, 0.0f, 5.0f}, 1.0f);
	float ray[3] = {0.0f, 0.0f, 1.0f};
	float origin[3] = {0.0f, 0.0f, 0.0f};
	float t = test_sphere(&sphere, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 5.0f, EPSILON_TEST));
}

int	add_sphere_tests(void)
{
	CU_pSuite	suite;

	suite = CU_add_suite("sphere_intersect", NULL, NULL);
	if (!suite)
		return (CU_get_error());
	if (!CU_add_test(suite, "test_sphere_hit_center", test_sphere_hit_center)
		|| !CU_add_test(suite, "test_sphere_from_inside", test_sphere_from_inside)
		|| !CU_add_test(suite, "test_sphere_miss", test_sphere_miss)
		|| !CU_add_test(suite, "test_sphere_tangent", test_sphere_tangent))
	{
		return (CU_get_error());
	}

	return (CUE_SUCCESS);
}
