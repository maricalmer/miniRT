/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:35:15 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 15:03:42 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

t_object	create_test_triangle(void)
{
	t_object obj;

	obj.type = TRI;
	obj.geo.tri.v0[0] = 0.0f; obj.geo.tri.v0[1] = 0.0f; obj.geo.tri.v0[2] = 0.0f;
	obj.geo.tri.v1[0] = 1.0f; obj.geo.tri.v1[1] = 0.0f; obj.geo.tri.v1[2] = 0.0f;
	obj.geo.tri.v2[0] = 0.0f; obj.geo.tri.v2[1] = 1.0f; obj.geo.tri.v2[2] = 0.0f;
	return (obj);
}

void	test_triangle_hit_center(void)
{
	t_object tri = create_test_triangle();
	float origin[3] = {0.3f, 0.3f, -1.0f};
	float ray[3] = {0.0f, 0.0f, 1.0f};
	float t = intersect_triangle(&tri, ray, origin);

	CU_ASSERT_TRUE(t > 0.0f);
}

void	test_triangle_miss_bounds(void)
{
	t_object tri = create_test_triangle();
	float origin[3] = {1.0f, 1.0f, -1.0f};
	float ray[3] = {0.0f, 0.0f, 1.0f};
	float t = intersect_triangle(&tri, ray, origin);

	CU_ASSERT_TRUE(assert_float_equal(t, 0.0f, EPSILON_TEST));
}

void	test_triangle_edge_hit(void)
{
	t_object tri = create_test_triangle();
	float origin[3] = {1.0f, 0.0f, -1.0f};
	float ray[3] = {0.0f, 0.0f, 1.0f};
	float t = intersect_triangle(&tri, ray, origin);

	CU_ASSERT_TRUE(t > 0.0f);
}

int	add_triangle_tests(void)
{
	CU_pSuite	suite;

	suite = CU_add_suite("triangle_intersect", NULL, NULL);
	if (!suite)
		return (CU_get_error());
	if (!CU_add_test(suite, "test_triangle_hit_center", test_triangle_hit_center)
		|| !CU_add_test(suite, "test_triangle_miss_bounds", test_triangle_miss_bounds)
		|| !CU_add_test(suite, "test_triangle_edge_hit", test_triangle_edge_hit))
	{
		return (CU_get_error());
	}

	return (CUE_SUCCESS);
}
