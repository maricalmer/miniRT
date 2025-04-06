/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cylinder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:26:31 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/06 19:30:30 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

t_object	create_test_cylinder(void)
{
	t_object	obj;

	obj.type = CYLINDER;
	obj.geo.cyl.center[0] = 0.0f;
	obj.geo.cyl.center[1] = 0.0f;
	obj.geo.cyl.center[2] = 0.0f;
	obj.geo.cyl.dir[0] = 0.0f;
	obj.geo.cyl.dir[1] = 0.0f;
	obj.geo.cyl.dir[2] = 1.0f;
	obj.geo.cyl.radius = 1.0f;
	obj.geo.cyl.height = 4.0f;
	return (obj);
}

void	test_cylinder_direct_hit(void)
{
	t_object obj = create_test_cylinder();
	float origin[3] = {2.0f, 0.0f, 0.0f};
	float ray[3] = {-1.0f, 0.0f, 0.0f};
	float t = test_cylinder(&obj, ray, origin);

	CU_ASSERT_TRUE(t > 0.0f);
}

void	test_cylinder_parallel_ray(void)
{
	t_object obj = create_test_cylinder();
	float origin[3] = {2.0f, 0.0f, -2.0f};
	float ray[3] = {0.0f, 0.0f, 1.0f};
	float t = test_cylinder(&obj, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 0.0f, EPSILON_TEST));
}

void test_cylinder_outside_height(void)
{
	t_object obj = create_test_cylinder();
	float origin[3] = {0.9f, 0.0f, 3.0f};
	float ray[3] = {-1.0f, 0.0f, 0.0f};
	float t = test_cylinder(&obj, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 0.0f, EPSILON_TEST));
}

void test_cylinder_angled_hit(void)
{
	t_object obj = create_test_cylinder();
	float origin[3] = {2.0f, 2.0f, 0.0f};
	float ray[3] = {-1.0f, -1.0f, 0.0f};
	float t = test_cylinder(&obj, ray, origin);

	CU_ASSERT_TRUE(t > 0.0f);
}

int	add_cylinder_tests(void)
{
	CU_pSuite suite = CU_add_suite("cylinder_intersect", 0, 0);

	if (!suite)
		return (1);
	if (!CU_add_test(suite, "test_cylinder_direct_hit", test_cylinder_direct_hit) ||
		!CU_add_test(suite, "test_cylinder_parallel_ray", test_cylinder_parallel_ray) ||
		!CU_add_test(suite, "test_cylinder_outside_height", test_cylinder_outside_height) ||
		!CU_add_test(suite, "test_cylinder_angled_hit", test_cylinder_angled_hit))
	{
		return (1);
	}
	return (0);
}