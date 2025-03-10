#include "minirt.h"

static void get_rodrigues_matrice(float u[3], float sin_theta, float cos_theta, float c[3], float r[4][4]);

void get_rotation_matrice(float cam_dir[3], float mat_rot[4][4], float c[3])
{
    // handle edgecase where v2 = v1 or v2 = -1 !!!!!!!!!!!!!!!!!!!!!!!!
    float   v1[3] = {0,0,-1};
    float   u[3];
    float   sin_theta;
    float   cos_theta;


    if (cam_dir[0] == 0 && cam_dir[1] == 0 && cam_dir[2] == -1)
    {
        ft_memset(mat_rot, 0, sizeof(float[4][4]));
        mat_rot[0][0] = 1;
        mat_rot[1][1] = 1;
        mat_rot[2][2] = 1;
        mat_rot[3][3] = 1;
        return ;
    }
    if (cam_dir[0] == 0 && cam_dir[1] == 0 && cam_dir[2] == 1)
    {
        ft_memset(mat_rot, 0, sizeof(float[4][4]));       
        mat_rot[0][0] = -1;
        mat_rot[1][1] = 1;
        mat_rot[2][2] = -1;
        mat_rot[3][3] = 1;
        return ;
    } 
    cprod_13_13(v1, cam_dir, u);
    normalize2(u, &sin_theta);
    cos_theta = dot_13_13(v1, cam_dir);

    get_rodrigues_matrice(u, sin_theta, cos_theta, c, mat_rot);
}

void rodrigues_matrice_handler(float u[3], float theta, float c[3], float r[4][4])
{
    float   sin_theta;
    float   cos_theta; 
    
    sin_theta = sin(theta);
    cos_theta = cos(theta);
    get_rodrigues_matrice(u, sin_theta, cos_theta, c, r);
}

static void get_rodrigues_matrice(float u[3], float sin_theta, float cos_theta, float c[3], float r[4][4])
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
    ft_memset(r[3], 0, sizeof(float[3]));
    r[3][3] = 1;
}


void first_rotation_matrice(t_data *data)
{
    float   tmp[3];
    
    ft_memset(data->cam.x , 0, sizeof(float[3]));
    data->cam.x[0] = 1;
    ft_memset(data->cam.y , 0, sizeof(float[3]));
    data->cam.y[1] = 1;

    get_rotation_matrice(data->cam.direction, data->cam.mat_rot, data->cam.world_center);
    cpy_vec(data->cam.y, tmp);
    
    
    dot_inplace_34_13(data->cam.mat_rot, tmp);
    
    
    
    if (tmp[1] < 0)
    {
        float   R[4][4];

        ft_memset(R, 0, sizeof(float[4][4]));
        R[0][0] = -1;
        R[1][1] = -1;
        R[2][2] = 1;
        R[3][3] = 1;

        dot_inplace_44_44(data->cam.mat_rot, R);
    }
    dot_inplace_34_13(data->cam.mat_rot, data->cam.x);
	dot_inplace_34_13(data->cam.mat_rot, data->cam.y);
    dot_inplace_34_13(data->cam.mat_rot, data->cam.direction);
}

// void move_cam_origin_2(float cam_origin_backup[3], float R[4][4], float center[3], float cam_origin[3])
// {
//     float   p[3];

//     vec_substr(cam_origin_backup, center, p);
//     dot_inplace_34_13(R, p);
//     vec_add_inplace(p, center);
//     cpy_vec(p, cam_origin);
// }


// void move_cam_origin(float cam_origin_backup[3], float R[3][3], float center[3], float cam_origin[3])
// {
//     float   p[4];
//     float   c[4];
//     float   M[4][4];

//     ft_memcpy(p, cam_origin_backup, sizeof(float[3]));
//     p[3] = 1;
//     ft_memcpy(c, center, sizeof(float[3]));
//     c[3] = 1;

//     ft_memcpy(M[0], R[0], sizeof(float[3]));
//     ft_memcpy(M[1], R[1], sizeof(float[3]));
//     ft_memcpy(M[2], R[2], sizeof(float[3]));
//     ft_memset(M[3], 0, sizeof(float[3]));
//     // M[0][3] = c[0] - (R[0][0] * c[0] + R[0][1] * c[1] + R[0][2] * c[2]);
//     // M[1][3] = c[1] - (R[1][0] * c[0] + R[1][1] * c[1] + R[1][2] * c[2]);
//     // M[2][3] = c[2] - (R[2][0] * c[0] + R[2][1] * c[1] + R[2][2] * c[2]);
//     M[0][3] = c[0] * (1 - R[0][0])  - c[1] * R[0][1] - c[2] * R[0][2];
//     M[1][3] = -c[0] * R[1][0]  + c[1] * (1 - R[1][1]) - c[2] * R[1][2];
//     M[2][3] = -c[0] * R[2][0]  - c[1] * R[2][1] + c[2] * (1 - R[2][2]);
//     M[3][3] = 1;
//     dot_inplace_44_14(M, p);
//     cam_origin[0] = p[0];
//     cam_origin[1] = p[1];
//     cam_origin[2] = p[2];
// }