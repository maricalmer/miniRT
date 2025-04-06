/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_plane.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:16:27 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/06 19:22:38 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

t_object	create_test_plane(void)
{
	t_object	obj;

	obj.type = PLANE;
	obj.geo.pl.point[0] = 0.0f;
	obj.geo.pl.point[1] = 0.0f;
	obj.geo.pl.point[2] = 5.0f;
	obj.geo.pl.normal[0] = 0.0f;
	obj.geo.pl.normal[1] = 0.0f;
	obj.geo.pl.normal[2] = 1.0f;
	return (obj);
}

void	test_plane_hit_directly(void)
{
	t_object obj = create_test_plane();
	float origin[3] = {0.0f, 0.0f, 0.0f};
	float ray[3] = {0.0f, 0.0f, 1.0f};
	float t = test_plane(&obj, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 5.0f, EPSILON_TEST));
}

void	test_plane_parallel_ray(void)
{
	t_object obj = create_test_plane();
	float origin[3] = {0.0f, 0.0f, 0.0f};
	float ray[3] = {1.0f, 0.0f, 0.0f};
	float t = test_plane(&obj, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 0.0f, EPSILON_TEST));
}

void	test_plane_back_facing_ray(void)
{
	t_object obj = create_test_plane();
	float origin[3] = {0.0f, 0.0f, 10.0f};
	float ray[3] = {0.0f, 0.0f, 1.0f};
	float t = test_plane(&obj, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 0.0f, EPSILON_TEST));
}

void	test_plane_origin_on_plane(void)
{
	t_object obj = create_test_plane();
	float origin[3] = {0.0f, 0.0f, 5.0f};
	float ray[3] = {0.0f, 0.0f, 1.0f};
	float t = test_plane(&obj, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 0.0f, EPSILON_TEST));
}

int	add_plane_tests(void)
{
	CU_pSuite suite = CU_add_suite("plane_intersect", 0, 0);

	if (!suite)
		return (1);
	if (!CU_add_test(suite, "test_plane_hit_directly", test_plane_hit_directly) ||
		!CU_add_test(suite, "test_plane_parallel_ray", test_plane_parallel_ray) ||
		!CU_add_test(suite, "test_plane_back_facing_ray", test_plane_back_facing_ray) ||
		!CU_add_test(suite, "test_plane_origin_on_plane", test_plane_origin_on_plane))
	{
		return (1);
	}
	return (0);
}
