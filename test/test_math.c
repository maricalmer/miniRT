/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_math.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 22:11:34 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/03 23:37:23 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	test_normalize(void)
{
	float	vec[3] = {3.0, 4.0, 0.0};
	float	magnitude = 0.0;

	normalize(vec, &magnitude);
	CU_ASSERT_TRUE(assert_float_equal(magnitude, 5.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(vec[0], 3.0 / 5.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(vec[1], 4.0 / 5.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(vec[2], 0.0, EPSILON_TEST));
}

void	test_dot_13_13(void)
{
	float	a[3] = {1.0, 2.0, 3.0};
	float	b[3] = {4.0, -5.0, 6.0};
	float	result = dot_13_13(a, b);

	CU_ASSERT_TRUE(assert_float_equal(result, 12.0, EPSILON_TEST));
}

void	test_vec_substr(void)
{
	float	p1[3] = {1.0, 2.0, 3.0};
	float	p2[3] = {4.0, 6.0, 8.0};
	float	result[3];

	vec_substr(p2, p1, result);
	CU_ASSERT_TRUE(assert_float_equal(result[0], 3.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(result[1], 4.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(result[2], 5.0, EPSILON_TEST));
}

void	test_cprod_13_13(void)
{
	float	a[3] = {1.0, 0.0, 0.0};
	float	b[3] = {0.0, 1.0, 0.0};
	float	result[3];

	cprod_13_13(a, b, result);
	CU_ASSERT_TRUE(assert_float_equal(result[0], 0.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(result[1], 0.0, EPSILON_TEST));
	CU_ASSERT_TRUE(assert_float_equal(result[2], 1.0, EPSILON_TEST));
}

void	test_triple_scalar(void)
{
	float	a[3] = {1.0, 0.0, 0.0};
	float	b[3] = {0.0, 1.0, 0.0};
	float	c[3] = {0.0, 0.0, 1.0};
	float	result = triple_scalar(a, b, c);

	CU_ASSERT_TRUE(assert_float_equal(result, 1.0, EPSILON_TEST));
}

int	add_math_tests(void)
{
	CU_pSuite	suite;

	suite = CU_add_suite("math", NULL, NULL);
	if (!suite)
		return (CU_get_error());
	if (!CU_add_test(suite, "test_normalize", test_normalize)
		|| !CU_add_test(suite, "test_dot_13_13", test_dot_13_13)
		|| !CU_add_test(suite, "test_vec_substr", test_vec_substr)
		|| !CU_add_test(suite, "test_cprod_13_13", test_cprod_13_13)
		|| !CU_add_test(suite, "test_triple_scalar", test_triple_scalar))
	{
		return (CU_get_error());
	}

	return (CUE_SUCCESS);
}
