/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:49:25 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/18 01:11:01 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SETTINGS_H
# define SETTINGS_H

# define WIDTH					800
# define HEIGHT					800
# define EPSILON    			0.01f // adjust to scene size
# define SPECULAR_POWER 		50
# define FRESNEL_TOLERANCE		0.02f
# define DEPTH_MAX				50
# define ANTIALIASING_FACT		3

# define SKY_COLOR_R			176
# define SKY_COLOR_G			221
# define SKY_COLOR_B			252

# define CAM_D_THETA			15
# define CAM_D_THETA_MOUSE		2
# define TRANS_SENS				8
# define MOVE_THRESHOLD_2		400

# define USLEEP_WORKER 			0
# define USLEEP_PARENT			100 //fine tune those...
# define N_THREAD				24

# define CROSS_CLICK_EVENT 		17
# define NO_EVENT_MASK			0

# undef FLT_MAX
# define FLT_MAX				1e+20F
# undef FLT_MIN
# define FLT_MIN				1e-20F

# define FAST_BVH_TRANSVERSAL	0 // fast is ok for convexe volumes, else strict (=> 0) should be used
# define MAX_BVH_GROUP			20
# define BVH_DEPTH_MAX			5
# define BVH_SIZE_MAX			37464 // (w^(d+1) - 1)/(w-1) + (w-1) + w
# define LEFT_HALF				0
# define RIGHT_HALF				1
# define ALGN_BVH_STR			32
# define ALGN_OBJ_ARR			64
# define BVH_CHILD_END			-2
# define BVH_LEAF				-1

# define RED_BG_START			"\033[41;1;37m"
# define RED_TXT_START			"\033[1;31m"
# define CYAN_TXT_START			"\033[1;36m"
# define PINK_TXT_START			"\033[1;95m"
# define COLOR_END				"\033[0m\n"

#endif
