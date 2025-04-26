/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:23:19 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 19:01:24 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
# define DATA_H

typedef struct s_data	t_data;

typedef enum e_err_msg
{
	ARGS_ERROR,
	FILE_ERROR,
	MAND_SET_ERROR,
	TYPES_ERROR,
	AMB_ERROR,
	CAM_ERROR,
	LIGHT_ERROR,
	SPHERE_ERROR,
	PLANE_ERROR,
	CYLINDER_ERROR,
	RECTANGLE_ERROR,
	TRI_ERROR,
	OBJ_ERROR
}	t_err_msg;

typedef enum e_obj_type
{
	PLANE,
	SPHERE,
	RECTANGLE,
	CYLINDER,
	TRI,
	BVH
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

typedef struct s_triangle
{
	float						v0[3];
	float						v1[3];
	float						v2[3];
	float						n0[3];
	float						n1[3];
	float						n2[3];
}	t_triangle;

typedef struct s_plane
{
	float						point[3];
	float						normal[3];
	float						u[3];
	float						v[3];
}	t_plane;

typedef struct s_rectangle
{
	float						point[3];
	float						normal[3];
	float						u[3];
	float						v[3];
	float						uv_size;
}	t_rectangle;

typedef struct s_material
{
	float						refr_idx;
	float						refr_coeff;
	float						refl_coeff;
	unsigned char				rgb[3];
	unsigned char				rgb2[3];
	float						checker_size;
}	t_material;

typedef union u_geo
{
	t_sphere				sph;
	t_cylinder				cyl;
	t_triangle				tri;
	t_plane					pl;
	t_rectangle				rec;
	void					*bvh;
}	t_geo;

typedef struct s_object 
{
	t_obj_type					type;
	t_geo						geo;
	t_material					mat;
	int							padding[6];
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

// used for creating the bvh.
typedef struct s_obj_geo
{
	float	center[3];
	float	bmin[3];
	float	bmax[3];
}	t_obj_geo;

// start of bvh helper struct
typedef struct s_cut_in_two
{
	int			axis;
	float		mid[3];
	int			idx_left;
	int			idx_right;
	int			old_n_obj;
	t_object	**old_group;
	t_obj_geo	**old_geo;
}	t_cut_in_two;

typedef struct s_bvh_stats
{
	int	max_depth;
	int	n_nodes;
	int	n_obj;
	int	n_leaves;
	int	min;
	int	max;
}	t_bvh_stats;

typedef struct s_bbox
{
	float	min[3];
	float	max[3];
}	t_bbox;
// end of bvh helper struct

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
	t_object					*all_objects;
	int							n_obj;
	t_light						*lights;
	int							n_light;
	int							n_obj_files;
	char						**obj_files;
	t_ambient_light				ambient;
	t_camera					cam;
	int							mouse_pressed_l;
	int							mouse_pressed_r;
	int							mouse_x;
	int							mouse_y;
	int							anti_fa;

	atomic_int					next_job_id;
	atomic_int					active_threads;
	t_calc_img_arg				joblist[HEIGHT];
	pthread_mutex_t				joblist_mutex;
	pthread_t					threads[N_THREAD];
	atomic_int					exit;

	t_mlxlib					mlx;
	t_obj_geo					*bvh_geo_data;

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

typedef struct s_intersect_result
{
	float						min;
	float						max;
}	t_intersect_result;

typedef struct s_bvh
{
	float						min_x[BVH_SIZE_MAX];
	float						min_y[BVH_SIZE_MAX];
	float						min_z[BVH_SIZE_MAX];
	float						max_x[BVH_SIZE_MAX];
	float						max_y[BVH_SIZE_MAX];
	float						max_z[BVH_SIZE_MAX];
	int							children[BVH_SIZE_MAX];
	t_object					**group[BVH_SIZE_MAX];
	int							group_size[BVH_SIZE_MAX];
	int							depth[BVH_SIZE_MAX];
	t_obj_geo					**obj_geo[BVH_SIZE_MAX];
}	t_bvh;

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

typedef struct s_aabb_simd
{
	__m256						bbox;
	__m256						v_dir;
	__m256						v_src;
	__m256						odd;
	__m256						tmp;
	__m256						t1;
	__m256						t2;
	__m256						mask;
	__m256						min;
	__m256						max;
}	t_aabb_simd;

#endif
