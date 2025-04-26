/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_find_median.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 23:01:24 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 15:49:37 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

# define MAX_BVH_TEST_SIZE 10

static t_bvh*	create_mock_bvh(int size)
{
	t_bvh	*bvh = malloc(sizeof(t_bvh));
	bvh->group_size[0] = 5;
	for (int i = 0; i < MAX_BVH_TEST_SIZE; i++)
	{
		bvh->group_size[i] = size;
		bvh->obj_geo[i] = malloc(sizeof(t_obj_geo *) * bvh->group_size[i]);
		bvh->group[i] = malloc(sizeof(t_object *) * bvh->group_size[i]);
		for (int j = 0; j < bvh->group_size[i]; j++)
		{
			bvh->obj_geo[i][j] = malloc(sizeof(t_obj_geo));
			bvh->obj_geo[i][j]->center[0] = 1.0f * j;
			bvh->obj_geo[i][j]->center[1] = 2.0f * j;
			bvh->obj_geo[i][j]->center[2] = 3.0f * j;
			bvh->group[i][j] = malloc(sizeof(t_object));
		}
	}
	return (bvh);
}

void	test_find_bvh_median_odd(void)
{
	t_bvh	*bvh = create_mock_bvh(5);
	int		idx = 0;
	int		axis = 0;
	float	median = find_bvh_median(bvh, idx, axis);

	CU_ASSERT_TRUE(assert_float_equal(median, 2.0, EPSILON_TEST));
}

void	test_find_bvh_median_even(void)
{
	t_bvh	*bvh = create_mock_bvh(6);
	int		idx = 0;
	int		axis = 0;
	float	median = find_bvh_median(bvh, idx, axis);

	CU_ASSERT_TRUE(assert_float_equal(median, 2.5, EPSILON_TEST));
}

int	add_find_median_tests(void)
{
	CU_pSuite	suite;

	suite = CU_add_suite("find_median", NULL, NULL);
	if (!suite)
		return (CU_get_error());
	if (!CU_add_test(suite, "test_find_bvh_median_odd", test_find_bvh_median_odd)
		|| !CU_add_test(suite, "test_find_bvh_median_even", test_find_bvh_median_even))
	{
		return (CU_get_error());
	}

	return (CUE_SUCCESS);
}

