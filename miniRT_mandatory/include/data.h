/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:11:23 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/03/26 12:40:54 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
# define DATA_H

typedef struct s_data	t_data;

typedef enum e_obj_type
{
	PLANE,
	SPHERE,
	CYLINDER
}	t_obj_type;

typedef struct s_camera
{
	int							fov;
	float						origin[3];
	float						origin_backup[3];
	float						direction[3];
	double						t_mat[4][4];
	float						r_mat[3][3];
	float						r_mat_0[3][3];
	float						world_center[3];
	float						x[3];
	float						y[3];
	float						z[3];
	float						d_trans;
	int							mode;
}	t_camera;

typedef struct s_light
{
	float						brightness;
	float						origin[3];
	float						rgb[3];
}	t_light;

typedef struct s_ambient_light
{
	float						brightness;
	float						rgb[3];
}	t_ambient_light;

typedef struct s_sphere
{
	float						center[3];
	float						radius;
}	t_sphere;

typedef struct s_cylinder
{
	float						center[3];
	float						radius;
	float						dir[3];
	float						height;
}	t_cylinder;

typedef struct s_plane
{
	float						point[3];
	float						normal[3];
	float						u[3];
	float						v[3];
}	t_plane;

typedef struct s_material
{
	unsigned char				rgb[3];
}	t_material;

typedef union u_geo
{
	t_sphere				sph;
	t_cylinder				cyl;
	t_plane					pl;
}	t_geo;

typedef struct s_object 
{
	t_obj_type					type;
	t_geo						geo;
	t_material					mat;
}	t_object;

typedef struct s_ray_prim
{
	float						max_x;
	float						max_y;
	float						dx;
	float						hd_dx;
}	t_ray_prim;

typedef struct s_mlxlib 
{
	void						*mlx;
	void						*win;
	void						*img;
	int							*addr;
	int							bpp;
	int							endian;
	int							line_len;
}	t_mlxlib;

typedef struct s_calc_img_arg
{
	t_data						*data;
	float						dx;
	float						dx_hd;
	float						x_tl;
	float						y_tl;
	int							line;
}	t_calc_img_arg;

typedef struct s_data
{
	t_object					*objects;
	int							objects_idx;	
	int							n_obj;
	t_light						*lights;
	int							n_light;
	int							n_obj_files;
	char						**obj_files;
	t_ambient_light				ambient;
	t_camera					cam;
	float						*primary_rays;
	int							mouse_pressed_l;
	int							mouse_pressed_r;
	int							mouse_x;
	int							mouse_y;
	int							anti_fa;
	atomic_int					joblist_top;
	atomic_int					active_threads;
	t_calc_img_arg				joblist[HEIGHT];
	atomic_int					exit;
	t_mlxlib					mlx;
	int							rt_fd;
	int							obj_fd;
}	t_data;

typedef struct s_shoot
{
	float						*src;
	float						dir[3];
	int							depth;
	int							inside;
	t_object					*obj;
	float						normal[3];
	float						hit_pt[3];
	float						shadow_ray[3];
	float						intens;
	int							res_rgb[3];
}	t_shoot;

typedef struct s_obj_parser
{
	float						(*normals)[3];
	float						(*vertices)[3];
	int							**faces;
	int							n_v;
	int							n_vn;
	int							n_f;
	int							idx_v;
	int							idx_n;
	char						*filename;
	int							tri_rgb[3];
	float						tri_refr_idx;
	float						tri_refr_coeff;
	float						tri_refl_coeff;
}	t_obj_parser;

#endif