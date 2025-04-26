/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_rectangle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:54:06 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 15:06:00 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static t_object	create_test_rectangle(void)
{
	t_object	obj;

	obj.geo.rec.point[0] = 0.0f;
	obj.geo.rec.point[1] = 0.0f;
	obj.geo.rec.point[2] = 5.0f;
	obj.geo.rec.normal[0] = 0.0f;
	obj.geo.rec.normal[1] = 0.0f;
	obj.geo.rec.normal[2] = -1.0f;
	obj.geo.rec.u[0] = 1.0f; obj.geo.rec.u[1] = 0.0f; obj.geo.rec.u[2] = 0.0f;
	obj.geo.rec.v[0] = 0.0f; obj.geo.rec.v[1] = 1.0f; obj.geo.rec.v[2] = 0.0f;
	obj.geo.rec.uv_size = 1.0f;

	return (obj);
}

void	test_rectangle_hit_center(void)
{
	t_object rect = create_test_rectangle();
	float origin[3] = {0.0f, 0.0f, 0.0f};
	float ray[3] = {0.0f, 0.0f, 1.0f};
	float t = intersect_rectangle(&rect, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 5.0f, EPSILON_TEST));
}

void	test_rectangle_parallel_ray(void)
{
	t_object rect = create_test_rectangle();
	float origin[3] = {0.0f, 0.0f, 0.0f};
	float ray[3] = {1.0f, 0.0f, 0.0f};
	float t = intersect_rectangle(&rect, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 0.0f, EPSILON_TEST));
}

void	test_rectangle_miss_bounds(void)
{
	t_object rect = create_test_rectangle();
	float origin[3] = {5.0f, 5.0f, 0.0f};
	float ray[3] = {0.0f, 0.0f, 1.0f};
	float t = intersect_rectangle(&rect, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 0.0f, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(intersect_rectangle(&rect, ray, origin), 0.0f, EPSILON_TEST));
}

void	test_rectangle_opposite_direction(void)
{
	t_object rect = create_test_rectangle();
	float origin[3] = {0.0f, 0.0f, 0.0f};
	float ray[3] = {0.0f, 0.0f, -1.0f};
	float t = intersect_rectangle(&rect, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 0.0f, EPSILON_TEST));
}

int	add_rectangle_tests(void)
{
	CU_pSuite suite = CU_add_suite("rectangle_intersect", 0, 0);

	if (!suite)
		return (1);
	if (!CU_add_test(suite, "test_rectangle_hit_center", test_rectangle_hit_center) ||
		!CU_add_test(suite, "test_rectangle_parallel_ray", test_rectangle_parallel_ray) ||
		!CU_add_test(suite, "test_rectangle_miss_bounds", test_rectangle_miss_bounds) ||
		!CU_add_test(suite, "test_rectangle_opposite_direction", test_rectangle_opposite_direction))
	{
		return (1);
	}
	return (0);
}
