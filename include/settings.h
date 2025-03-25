#ifndef SETTINGS_H
# define SETTINGS_H

# define WIDTH					80
# define HEIGHT					80
# define EPSILON    			0.01f // adjust to scene size
# define SPECULAR_POWER 		50
# define FRESNEL_TOLERANCE		0.02f
# define DEPTH_MAX				50
# define ANTIALIASING_FACT		3

# define SKY_COLOR_R			176
# define SKY_COLOR_G			221
# define SKY_COLOR_B			252

// # define CAM_MODE				0 // 0 for object mode, 1 for flight mode		
# define CAM_D_THETA			15
// # define CAM_D_TRANS			10
# define CAM_D_THETA_MOUSE		2
// # define CAM_D_TRANS_MOUSE		5
# define TRANS_SENS				8
# define MOVE_THRESHOLD_2		400


# define USLEEP_WORKER 			0
# define USLEEP_PARENT			100 //fine tune those...
# define N_THREAD				20

# define CROSS_CLICK_EVENT 		17
# define NO_EVENT_MASK			0

# define FAST_BVH_TRANSVERSAL	0   // fast is ok for convexe volumes, else strict (=> 0) should be used.
# define MAX_BVH_GROUP			20
# define BVH_DEPTH_MAX			5
# define BVH_SIZE_MAX			37464 // more tricky ... (w^(d+1) - 1)/(w-1) + (w-1) + w

# define RED_BG_START			"\033[41;1;37m"
# define RED_TXT_START			"\033[1;31m"
# define CYAN_TXT_START			"\033[1;36m"
# define PINK_TXT_START			"\033[1;95m"
# define COLOR_END				"\033[0m\n"

#endif