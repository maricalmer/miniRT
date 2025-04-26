/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_dot_in_place_44.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:37:40 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 15:45:01 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	test_matrix44_multiply_inplace(void)
{
	double 	a[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
	double 	b[4][4] = {{16, 15, 14, 13}, {12, 11, 10, 9}, {8, 7, 6, 5}, {4, 3, 2, 1}};
	double 	expected[4][4] = {{80, 70, 60, 50}, {240, 214, 188, 162}, {400, 358, 316, 274}, {560, 502, 444, 386}};
	int		i;
	int		j;


	matrix44_multiply_inplace(a, b);
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			CU_ASSERT_TRUE(assert_float_equal(b[i][j], expected[i][j],
					EPSILON));
			j++;
		}
		i++;
	}
}

int	add_dot_in_place_44_tests(void)
{
	CU_pSuite	suite;

	suite = CU_add_suite("dot_in_place_44", NULL, NULL);
	if (!suite)
		return (CU_get_error());
	if (!CU_add_test(suite, "test_matrix44_multiply_inplace", test_matrix44_multiply_inplace))
		return (CU_get_error());
	return (CUE_SUCCESS);
}
