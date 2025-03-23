#ifndef MINIRT_H
# define MINIRT_H

# include <libft.h>
# include <stdlib.h>
# include <stdio.h>
# include <mlx.h>
# include <X11/keysym.h>
# include <X11/X.h>
# include <math.h>
# include <time.h>
# include <sys/time.h>
# include <float.h>
# include <stdio.h>
# include <pthread.h>
# include <stdatomic.h>
# include <fcntl.h>
# include <immintrin.h> // SSE/AVX and prefetch
# include <xmmintrin.h>

# define WIDTH					800
# define HEIGHT					800
# define EPSILON    			0.001f // adjust
# define SPECULAR_POWER 		50
# define FRESNEL_TOLERANCE		0.02f
# define DEPTH_MAX				6
# define ANTIALIASING_FACT		3

# define SKY_COLOR_R			70
# define SKY_COLOR_G			130
# define SKY_COLOR_B			180

# define CAM_D_THETA			15
# define CAM_D_TRANS			10
# define MOVE_THRESHOLD_2		400


# define USLEEP_WORKER 			0
# define USLEEP_PARENT			100 //fine tune those...
# define N_THREAD				20

# define CROSS_CLICK_EVENT 		17
# define NO_EVENT_MASK			0

# define BVH_ON					1
# define FAST_BVH_TRANSVERSAL	0   // fast is ok for convexe volumes, else strict (=> 0) should be used.
# define MAX_BVH_GROUP			20
# define BVH_DEPTH_MAX			5
# define BVH_SIZE_MAX			37464 // more tricky ... (w^(d+1) - 1)/(w-1) + (w-1) + w

# define CHECKER_SIZE			0.004

# define RED_BG_START			"\033[41;1;37m"
# define RED_TXT_START			"\033[1;31m"
# define CYAN_TXT_START			"\033[1;36m"
# define PINK_TXT_START			"\033[1;95m"
# define COLOR_END				"\033[0m\n"


typedef struct s_data t_data;

typedef enum e_obj_type
{
	PLANE,
	SPHERE,
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
	float						world_center[3];
}	t_camera;

typedef struct s_light
{
	float						brightness;
	float						origin[3];
	float						rgb[3]; // 0-1 value for each component
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
	// float						n[3];
	// float						u01[3];
	// float						u02[2];
	// float						u03[3];
	float						n0[3];
	float						n1[3];
	float						n2[3];
}	t_triangle;

typedef struct s_triangle2
{
	int 						v0_idx;
	int 						v1_idx;
	int 						v2_idx;
	int 						n0_idx;
	int 						n1_idx;
	int 						n2_idx;
}	t_triangle2;

typedef struct s_plane
{
	float						point[3];
	float						normal[3];
	float						u[3];
	float						v[3];
}	t_plane;

typedef struct s_material
{
	float						refr_idx;
	float						refr_coeff;
	float						refl_coeff;
	unsigned char				rgb[3];
	unsigned char				rgb2[3]; // NEEDS redesign to set rgb per object
	int							checker_flag;
}	t_material;

typedef struct s_object 
{
    t_obj_type					type;
    union 
	{
        t_sphere				sph;
        t_cylinder				cyl;
        t_triangle				tri;
        t_plane					pl;
		void					*bvh;
    } geo;
	t_material mat;
	int							padding[4];
}	t_object;

typedef struct s_ray_prim
{
	float						max_x;
	float						max_y;
	int							size_hd;
	float						dx;
	float						hd_dx;
}	t_ray_prim;

typedef struct	s_mlxlib {
	void						*mlx;
	void						*win;
	void						*img;
	int							*addr;
	int							bpp;
	int							endian;
	int							line_len;
}	t_mlxlib;


typedef	struct s_obj_geo  // used for creating the bvh.
{
	float	center[3];
	float	bmin[3];
	float	bmax[3];
}	t_obj_geo;


// start of bvh helper struct
typedef struct
{
	int			axis;
	float		mid[3];
	int			idx_left;
	int			idx_right;
	int			old_n_obj;
	t_object	**old_group;
	t_obj_geo	**old_geo;
}	t_cut_in_two;

typedef struct
{
    int max_depth;
    int n_nodes;
    int n_obj;
    int n_leafs;
    int min;
    int max;
}   t_bvh_stats;

typedef	struct s_bbox
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
	t_object					*objects;  // ==> list of pointers from the beginning ?!
	int							objects_idx;	
	t_object					*all_objects;  // needs to be updated
	int							n_obj;  // needs to be updated
	t_light						*lights;  // needs to be updated
	int							n_light;  // needs to be updated
	int							n_obj_files;
	char						**obj_files;
	t_ambient_light				ambient;  // needs to be updated
	char						*img_buf;
	// first shoot only
	t_camera					cam;  // needs to be updated
	float						*primary_rays; // can be removed.
	int							mouse_pressed_l;
	int							mouse_pressed_r;
	int							mouse_x;
	int							mouse_y;
	int							antialiasing_fact;

	// multithreading
	atomic_int					joblist_top;
	atomic_int					active_threads;
	t_calc_img_arg				joblist[HEIGHT];
	pthread_mutex_t				joblist_mutex;
	pthread_t					threads[N_THREAD];
	atomic_int					exit;
	//mlx
	t_mlxlib					mlx;
	//bvh_creation
	t_obj_geo					*bvh_geo_data;
	int							is_bvh;
	//parsing
	int							rt_fd;
	int							obj_fd;
	// tri
	int							tri_idx;
	float						(*normals)[3];
	float						(*vertices)[3];
}	t_data;

typedef struct s_shoot
{
	// input
	float						*src;
	float						dir[3];
	int							depth;
	int							inside;
	// intermediate
	t_object					*obj;
	float						normal[3];
	float						hit_pt[3];
	float						shadow_ray[3];
	// output
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
	int							childs[BVH_SIZE_MAX]; //index of the first child
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
}   t_obj_parser;

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


/// FUNCTIONS
/*prints.c*/
void							print_intro(void);
void							print_outro(void);
void    						print_bvh_stats(t_bvh *bvh);
void							print_bvh_build_t(clock_t start, clock_t end);
void							print_img_render_t(struct timeval t_start, struct timeval t_end);
void							print_tri_count(int counter);
/*checker.c*/
int								check_input(int ac, char **av, t_data *data);
/*identifiers.c*/
int								is_object_file(char *specs);
int								is_light(char *specs);
int								is_cam(char *specs);
int								is_ambient(char *specs);
int								is_plane(char *specs);
/*identifiers_2.c*/
int								is_sphere(char *specs);
int								is_cylinder(char *specs);
int								increase_if_uniq(int *value);
int								is_face(char *specs);
int								is_smoothing(char *specs);
/*init.c*/
int								init_rt_lists(t_data *data);
int								init_obj_lists(t_obj_parser *parser);
/*counter.c*/
int								read_rt(t_data *data);
int								read_obj(t_data *data, t_obj_parser *parser);
int								count_rt_elems(char *specs, t_data *data, int *n_cam, int *n_ambient);
/*obj_file*/
int								parse_obj_files(t_data *data, char *filename);
int								read_obj_file(t_data *data, t_obj_parser *parser);
int								process_obj_line(char *specs, t_data *data, t_obj_parser *parser);
/*rt_file*/
int								create_elements_rt(t_data *data, char *filename);
/*parser.c*/
int								handle_parsing(char **av, t_data *data);
char							*format_string(char *str, int len);
/*factories.c*/
int								create_ambient_light(t_data *data, char *specs);
int								create_cam(t_data *data, char *specs);
int								create_light(t_data *data, char *specs);
int								create_sphere(t_data *data, char *specs);
int								create_plane(t_data *data, char *specs);
int								create_cylinder(t_data *data, char *specs);
int								create_triangle(t_data *data, char *line, t_obj_parser *parser);
int								set_tri(t_obj_parser *parser, char *specs);
/*factories_utils.c*/
int								get_ratio(char **specs, float *ratio);
int								get_refr_idx(char **specs, float *ratio);
int								get_rgb_norm(char **specs, float *color);
int								get_rgb(char **specs, unsigned char *color);
int								get_obj_rgb(char **specs, int *color);
int								get_coord(char **specs, float *value);
int								get_vec_norm(char **specs, float *value);
int								get_fov_range(char **specs, int *fov);
int								get_radius(char **specs, float *radius);
int								get_length(char **specs, float *length);
int								get_checkerboard_flag(char **specs, int *flag);
/*error.c*/
void							print_error(int errnum);
/*free.c*/
void    						free_bvh_1(t_bvh *bvh);
void    						free_bvh_2(t_bvh *bvh);
void							free_data(t_data *data);
void							join_threads(t_data *data);
void    						free_obj_parse_1(t_obj_parser *parsers, int n_files);
void							free_obj_parse_2(t_obj_parser *parsers, int n_files);
void							free_at_n_and_v(t_obj_parser *parsers, int n_files);
/*dummy_parsing.c --- TO DELETE */
void							parsing(t_data *data);


/*render.c*/
void							render_first_image(t_data *data);
void							shoot_ray(t_data *data, t_shoot *shoot);
void 							first_rotation_matrice(t_data *data);
void 							calculate_img(t_data *data);
// void 		move_cam_origin(float cam_origin_backup[3], float R[3][3], float center[3], float cam_origin[3]);

/*checkerboard.c*/
int								check_checkerboard_grid(t_shoot *shoot);

/*phong.c*/
void							shading(t_shoot *shoot, t_data *data);

/* BVH */
t_bvh   						*init_bvh(t_data *data);
void							update_group(t_data *data, t_bvh *bvh);
void							cut_in_two(t_bvh *bvh, int idx, int idx_c, int i);
void							get_group_size(t_data *data, t_bvh *bvh);
void							create_obj_list_root(t_data *data, t_bvh *bvh);
int								find_min_idx(int x[3]);
void 							malloc_groups_n_geo(t_bvh *bvh, t_cut_in_two *cut);
void							get_mid_planes(t_bvh *bvh, int idx, t_cut_in_two *cut);
t_obj_geo 						*create_obj_geo_data(t_bvh *bvh);
void 							get_bboxes(t_bvh *bvh, int idx, t_cut_in_two *cut);
void							get_bbox_min_max_root(t_bvh *bvh);


/* tests*/
float							visi_test_bvh_strict(t_bvh *bvh, int idx, t_shoot *shoot);
float							visi_test_bvh_fast(t_bvh *bvh, int idx, t_shoot *shoot);
float							shadow_test_bvh(t_shoot *shoot, t_bvh *bvh, int idx, float dist_light);
float							visi_tests(t_object *objects, t_shoot *shoot, int n_obj);
float 							visi_test_leafs(t_object **objects, t_shoot *shoot, int n_obj);
float							shadow_tests(t_shoot *shoot, t_object *objects, float dist_light, int n_obj);
float 							shadow_test_leafs(t_shoot *shoot, t_object **objects, float dist_light, int n_obj);
float							test_sphere(t_object *obj, float ray[3], float origin[3]);
// float							test_cylinder(t_object *obj, t_shoot *shoot, int shadow_flag);
float	test_cylinder(t_object *obj, float ray[3], float origin[3]);
float	intersection_test_cylinder(t_object *obj, float ray[3], float origin[3]);
// float							test_cylinder(t_object *obj, float ray[3], float origin[3]);
float							test_plane(t_object *obj, float p_ray[3], float origin[3]);
float							test_triangle(t_object *obj, float ray[3], float origin[3]);

__m256 							aabb_test_simd(t_bvh *bvh, int idx, float dir[3], float src[3]);
void 							aabb_test_fast(t_bvh *bvh, int idx, t_shoot *shoot, char res[9]);
void							copy_and_terminate(char *res, int *indices, int size);

/*maths*/
float							dot_13_13(float a[3], float b[3]);
void							dot_inplace_34_13(double a[3][4], float b[3]);
void							dot_inplace_33_13(float a[3][3], float b[3]);
void							dot_inplace_44_44(double a[4][4], double b[4][4]);
void							cprod_13_13(float a[3], float b[3], float res[3]);
void							normalize(float vector[3], float *magnitude);
int								imin(int a, int b);
int								imax(int a, int b);
void							vec_substr(float p1[3], float p2[3], float result[3]);
void							cpy_vec(float v1[3], float v2[3]);
float 							triple_scalar(float a[3], float b[3], float c[3]);
void							ft_swap(float *t1, float *t2);
float							find_median_custom(t_bvh *bvh, int idx, int axis);
float 							find_median2(float centers[3], int n); // change this GPT code !!!
void 							get_rotation_matrice(float cam_dir[3], double t_mat[4][4], float c[3]);
void 							copy_r_mat(t_data *data);
void 							rodrigues_matrice_handler(float u[3], float theta, float c[3], double r[4][4]);
void							scale_vec(float v[3], float amp);

/* Multithreading */
void							wait_for_workers(t_data *data);
void							launch_pool(t_data *data);
void							*worker(void *arg);
void 							calculate_img_packet(t_calc_img_arg *arg);

/* Perf */
void							print_render_stats(double render_time);
double							stop_timer(clock_t start);
void							start_timer(clock_t *start);
double							get_time();

/* mlx n events */
int								handle_input(int keysym, t_data *data);
int								handle_close(t_data *data);
int								init_mlx(t_mlxlib *data);
void 							rotate_cam(t_data *data, float theta, float axis[3], int anti_fa);
void 							translate_cam(t_data *data, float v[3], float amp, int anti_fa);
int mouse_press(int button, int x, int y, void *arg);
int mouse_release(int button, int x, int y, void *arg);
int mouse_move(int x, int y, void *arg);

#endif
