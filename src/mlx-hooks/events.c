#include "minirt.h"

void copy_r_mat(t_data *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < 3)
	{
		j = -1;
		while (++j < 3)
			data->cam.r_mat[i][j] = data->cam.t_mat[i][j];
	}
}

void translate_cam(t_data *data, float v[3], float amp, int anti_fa)
{
	float t[3];
	double R[4][4]; // change that name

	data->antialiasing_fact = anti_fa;
	cpy_vec(v, t);
	scale_vec(t, amp);
	ft_memset(R, 0, sizeof(double[4][4]));
	R[0][0] = R[1][1] = R[2][2] = R[3][3] = 1;
	R[0][3] = t[0];
	R[1][3] = t[1];
	R[2][3] = t[2];
	dot_inplace_44_44(data->cam.t_mat, R);
	copy_r_mat(data);
	ft_memcpy(data->cam.origin, data->cam.origin_backup, sizeof(float[3]));
	dot_inplace_34_13(data->cam.t_mat, data->cam.origin);
	calculate_img(data);
}

void rotate_cam(t_data *data, float theta, float axis[3], int anti_fa)
{
	double R[4][4]; // change that name
	
	data->antialiasing_fact = anti_fa;
	rodrigues_matrice_handler(axis, theta, data->cam.world_center, R);
	dot_inplace_44_44(data->cam.t_mat, R); // update the global transformation matrice
	copy_r_mat(data);
	ft_memcpy(data->cam.origin, data->cam.origin_backup, sizeof(float[3]));
	dot_inplace_34_13(data->cam.t_mat, data->cam.origin);
	calculate_img(data);
}

