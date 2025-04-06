/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_rodrigues.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 00:28:56 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/05 19:33:08 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static int	assert_double_matrices_equal(double mat1[4][4], double mat2[4][4])
{
	int	i;
	int	j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			if (fabs(mat1[i][j] - mat2[i][j]) > EPSILON_TEST)
				return (0);
		}
	}
	return (1);
}

void	test_get_rotation_matrice_basic(void)
{
	t_data	data;
	double	expected_matrix[4][4] = {{-1.0, 0, 0, 0}, {0, 1.0, 0, 0}, {0, 0, -1.0, 0}, {0, 0, 0, 1.0}};

	data.cam.direction[0] = 0.0f;
	data.cam.direction[1] = 0.0f;
	data.cam.direction[2] = 1.0f;
	memset(data.cam.t_mat, 0, sizeof(double) * 16);
	data.cam.t_mat[0][0] = 1;
	data.cam.t_mat[1][1] = 1;
	data.cam.t_mat[2][2] = 1;
	data.cam.t_mat[3][3] = 1;
	get_rotation_matrice(data.cam.direction, data.cam.t_mat, (float [3]){0.0f, 0.0f, 0.0f});
	CU_ASSERT_TRUE(assert_double_matrices_equal(data.cam.t_mat, expected_matrix));
}

void	test_get_rotation_matrice_normal_with_flip(void)
{
	t_data	data;
	double	expected_matrix[4][4] = {{0, 0, -1, 0}, {0, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 1}};

	data.cam.direction[0] = 1.0f;
	data.cam.direction[1] = 0.0f;
	data.cam.direction[2] = 0.0f;
	memset(data.cam.t_mat, 0, sizeof(double) * 16);
	data.cam.t_mat[0][0] = 1;
	data.cam.t_mat[1][1] = 1;
	data.cam.t_mat[2][2] = 1;
	data.cam.t_mat[3][3] = 1;
	get_rotation_matrice(data.cam.direction, data.cam.t_mat, (float [3]){0.0f, 0.0f, 0.0f});
	CU_ASSERT_TRUE(assert_double_matrices_equal(data.cam.t_mat, expected_matrix));
}

void	test_get_rotation_matrice_special_case_minus_z(void)
{
	t_data	data;
	double	identity_matrix[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

	data.cam.direction[0] = 0.0f;
	data.cam.direction[1] = 0.0f;
	data.cam.direction[2] = -1.0f;
	memset(data.cam.t_mat, 0, sizeof(double) * 16);
	data.cam.t_mat[0][0] = 1;
	data.cam.t_mat[1][1] = 1;
	data.cam.t_mat[2][2] = 1;
	data.cam.t_mat[3][3] = 1;
	get_rotation_matrice(data.cam.direction, data.cam.t_mat, (float [3]){0.0f, 0.0f, 0.0f});
	CU_ASSERT_TRUE(assert_double_matrices_equal(data.cam.t_mat, identity_matrix));
}

void	test_get_rotation_matrice_special_case_plus_z(void)
{
	t_data	data;
	double	special_case_plus_z_matrix[4][4] = {{-1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, -1, 0},{0, 0, 0, 1}};

	data.cam.direction[0] = 0.0f;
	data.cam.direction[1] = 0.0f;
	data.cam.direction[2] = 1.0f;
	memset(data.cam.t_mat, 0, sizeof(double) * 16);
	data.cam.t_mat[0][0] = 1;
	data.cam.t_mat[1][1] = 1;
	data.cam.t_mat[2][2] = 1;
	data.cam.t_mat[3][3] = 1;
	get_rotation_matrice(data.cam.direction, data.cam.t_mat, (float [3]){0.0f, 0.0f, 0.0f});
	CU_ASSERT_TRUE(assert_double_matrices_equal(data.cam.t_mat, special_case_plus_z_matrix));
}

void	test_rodrigues_matrice_handler(void)
{
	float	u[3] = {0.0f, 0.0f, 1.0f};
	float	theta = M_PI / 2;
	float	c[3] = {1.0f, 1.0f, 1.0f};
	double	r[4][4];
	double	expected_rotation_matrix[4][4] = {{0, -1, 0, 2}, {1, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

	rodrigues_matrice_handler(u, theta, c, r);
	CU_ASSERT_TRUE(assert_double_matrices_equal(r, expected_rotation_matrix));
}

int	add_rodrigues_tests(void)
{
	CU_pSuite	suite;

	suite = CU_add_suite("add_rodrigues_tests", NULL, NULL);
	if (!suite)
		return (CU_get_error());
	if (!CU_add_test(suite, "test_get_rotation_matrice_basic", test_get_rotation_matrice_basic)
		|| !CU_add_test(suite, "test_get_rotation_matrice_normal_with_flip", test_get_rotation_matrice_normal_with_flip)
		|| !CU_add_test(suite, "test_get_rotation_matrice_special_case_minus_z", test_get_rotation_matrice_special_case_minus_z)
		|| !CU_add_test(suite, "test_get_rotation_matrice_special_case_plus_z", test_get_rotation_matrice_special_case_plus_z)
		|| !CU_add_test(suite, "test_rodrigues_matrice_handler", test_rodrigues_matrice_handler))
	{
		return (CU_get_error());
	}

	return (CUE_SUCCESS);
}
