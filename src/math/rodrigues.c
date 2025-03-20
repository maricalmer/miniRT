#include "minirt.h"

static void get_rodrigues_matrice(float u[3], float sin_theta, float cos_theta, float c[3], double r[4][4]);

void get_rotation_matrice(float cam_dir[3], double t_mat[4][4], float c[3])
{
    float   v1[3] = {0,0,-1};
    float   u[3];
    float   sin_theta;
    float   cos_theta;

    if (cam_dir[0] == 0 && cam_dir[1] == 0 && cam_dir[2] == -1)
    {
        ft_memset(t_mat, 0, sizeof(double[4][4]));
        t_mat[0][0] = 1;
        t_mat[1][1] = 1;
        t_mat[2][2] = 1;
        t_mat[3][3] = 1;
        return ;
    }
    if (cam_dir[0] == 0 && cam_dir[1] == 0 && cam_dir[2] == 1)
    {
        ft_memset(t_mat, 0, sizeof(double[4][4]));       
        t_mat[0][0] = -1;
        t_mat[1][1] = 1;
        t_mat[2][2] = -1;
        t_mat[3][3] = 1;
        return ;
    } 
    cprod_13_13(v1, cam_dir, u);
    normalize(u, &sin_theta);
    cos_theta = dot_13_13(v1, cam_dir);
    get_rodrigues_matrice(u, sin_theta, cos_theta, c, t_mat);
}

void rodrigues_matrice_handler(float u[3], float theta, float c[3], double r[4][4])
{
    float   sin_theta;
    float   cos_theta; 
    
    sin_theta = sin(theta);
    cos_theta = cos(theta);
    get_rodrigues_matrice(u, sin_theta, cos_theta, c, r);
}

static void get_rodrigues_matrice(float u[3], float sin_theta, float cos_theta, float c[3], double r[4][4])
{ 
    r[0][0] = cos_theta + u[0] * u[0] * (1 - cos_theta);
    r[0][1] = u[0] * u[1] * (1 - cos_theta) - u[2] * sin_theta;
    r[0][2] = u[0] * u[2] * (1 - cos_theta) + u[1] * sin_theta;
    r[1][0] = u[0] * u[1] * (1 - cos_theta) + u[2] * sin_theta;
    r[1][1] = cos_theta + u[1] * u[1] * (1 - cos_theta);
    r[1][2] = u[1] * u[2] * (1 - cos_theta) - u[0] * sin_theta;
    r[2][0] = u[2] * u[0] * (1 - cos_theta) - u[1] * sin_theta;
    r[2][1] = u[2] * u[1] * (1 - cos_theta) + u[0] * sin_theta;
    r[2][2] = cos_theta + u[2] * u[2] * (1 - cos_theta);
    r[0][3] = c[0] * (1 - r[0][0])  - c[1] * r[0][1] - c[2] * r[0][2];
    r[1][3] = -c[0] * r[1][0]  + c[1] * (1 - r[1][1]) - c[2] * r[1][2];
    r[2][3] = -c[0] * r[2][0]  - c[1] * r[2][1] + c[2] * (1 - r[2][2]);
    ft_memset(r[3], 0, sizeof(double[3]));
    r[3][3] = 1;
}

void first_rotation_matrice(t_data *data)
{
    float   tmp[3];
    double   R[4][4];
    
    get_rotation_matrice(data->cam.direction, data->cam.t_mat,
        data->cam.world_center);
    cpy_vec((float[3]){0, 1, 0}, tmp);
    copy_r_mat(data);
    dot_inplace_33_13(data->cam.r_mat, tmp);
    if (tmp[1] < 0)
    {
        ft_memset(R, 0, sizeof(float[4][4]));
        R[0][0] = -1;
        R[1][1] = -1;
        R[2][2] = 1;
        R[3][3] = 1;
        dot_inplace_44_44(data->cam.t_mat, R);
    }
    copy_r_mat(data);
}

