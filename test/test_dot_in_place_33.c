/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_dot_in_place_33.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:37:13 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/03 23:25:18 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	assert_float_equal(float actual, float expected, float epsilon)
{
	if (isnan(actual) && isnan(expected))
		return (1);
	return (fabs(actual - expected) < epsilon);
}

void	test_dot_inplace_34_13(void)
{
	double a[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
	float b[3] = {1, 1, 1};

	dot_inplace_34_13(a, b);
	CU_ASSERT_TRUE(assert_float_equal(b[0], 10.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(b[1], 26.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(b[2], 42.0, EPSILON_TEST));
}

void	test_dot_inplace_33_13(void)
{
	float a[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
	float b[3] = {1, 1, 1};

	dot_inplace_33_13(a, b);
	CU_ASSERT_TRUE(assert_float_equal(b[0], 6.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(b[1], 15.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(b[2], 24.0, EPSILON_TEST));
}

void	test_dot_inplace_33_33(void)
{
	float a[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
	float b[3][3] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};

	dot_inplace_33_33(a, b);
	CU_ASSERT_TRUE(assert_float_equal(a[0][0], 30.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(a[0][1], 24.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(a[0][2], 18.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(a[1][0], 84.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(a[1][1], 69.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(a[1][2], 54.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(a[2][0], 138.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(a[2][1], 114.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(a[2][2], 90.0, EPSILON_TEST));
}

int	add_dot_in_place_33_tests(void)
{
	CU_pSuite	suite;

	suite = CU_add_suite("dot_in_place_33", NULL, NULL);
	if (!suite)
		return (CU_get_error());
	if (!CU_add_test(suite, "test_dot_inplace_33_33", test_dot_inplace_33_33)
		|| !CU_add_test(suite, "test_dot_inplace_33_13", test_dot_inplace_33_33)
		|| !CU_add_test(suite, "test_dot_inplace_34_13", test_dot_inplace_33_33))
	{
		return (CU_get_error());
	}
	return (CUE_SUCCESS);
}
