/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:29:09 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/03/26 13:06:41 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SETTINGS_H
# define SETTINGS_H

# define WIDTH					800
# define HEIGHT					800
# define EPSILON    			0.01f // adjust to scene size
# define SPECULAR_POWER 		50
# define ANTIALIASING_FACT		3

# define SKY_COLOR_R			176
# define SKY_COLOR_G			221
# define SKY_COLOR_B			252

# define CAM_D_THETA			15
# define CAM_D_THETA_MOUSE		2
# define TRANS_SENS				8
# define MOVE_THRESHOLD_2		400

# define CROSS_CLICK_EVENT 		17
# define NO_EVENT_MASK			0

# undef FLT_MAX
# define FLT_MAX				1e+20F
# undef FLT_MIN
# define FLT_MIN				1e-20F

# define RED_BG_START			"\033[41;1;37m"
# define RED_TXT_START			"\033[1;31m"
# define CYAN_TXT_START			"\033[1;36m"
# define PINK_TXT_START			"\033[1;95m"
# define COLOR_END				"\033[0m\n"

#endif