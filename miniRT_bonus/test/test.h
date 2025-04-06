/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:45:24 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/06 19:40:37 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include <CUnit/Basic.h>
# include <CUnit/CUnit.h>
# include "minirt.h"
# include "data.h"
# include "functions.h"
# include "settings.h"

# define EPSILON_TEST 1e-6

int		assert_float_equal(float actual, float expected, float epsilon);
int		add_dot_in_place_33_tests(void);
int		add_dot_in_place_44_tests(void);
int		add_math_tests(void);
int		add_find_median_tests(void);
int		add_rodrigues_tests(void);
int		add_checker_tests(void);
int		add_factories_tests(void);
int		add_factories2_tests(void);
int		add_factories3_tests(void);
int		add_factories4_tests(void);
int		add_sphere_tests(void);
int		add_rectangle_tests(void);
int		add_plane_tests(void);
int		add_cylinder_tests(void);
int		add_triangle_tests(void);

#endif