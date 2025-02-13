#ifndef MINIRT_H
# define MINIRT_H

# include "libft.h"
# include "stdlib.h"
# include "mlx.h"
# include <X11/keysym.h>
# include <X11/X.h>
# include <math.h>
# include <float.h>
# include <stdio.h>
# include <pthread.h>
# include <stdatomic.h>

# define WIDTH				1024
# define HEIGHT				1024
# define EPSILON    		0.001 // adjust
# define SPECULAR_POWER 	50
# define DEPTH_MAX			20
# define ANTIALIASING_FACT	4

# define MAX_BVH_GROUP		6


# define USLEEP_WORKER 		0
# define USLEEP_PARENT		10 //fine tune those...
# define N_THREAD			16


// extern atomic_int num_primary_rays;
// extern atomic_int num_object_tests;
// extern atomic_int num_object_intersections;
// extern int total_objects;

typedef enum e_obj_type
{
	END,
	SPHERE,
	PLANE
}	t_obj_type;

typedef struct s_camera
{
	int			fov;
	float		origin[3];
	float		direction[3];
}	t_camera;

typedef struct s_light
{
	float			brightness;
	float			origin[3];
	float			rgb[3]; // 0-1 value for each component
}	t_light;

typedef struct s_ambient_light
{
	float			brightness;
	float			rgb[3];
}	t_ambient_light;

typedef struct s_sphere
{
	float			center[3];
	float			radius;
}	t_sphere;

typedef struct s_plane
{
	float			point[3];
	float			normal[3];
}	t_plane;

typedef struct s_material
{
	float			refr_idx;
	float			refr_angle;
	float			refl_coeff;
	unsigned char	rgb[3];
}	t_material;

typedef struct s_object
{
	t_obj_type		type;
	t_material		mat;
	void			*geo; // union option (maybe faster / use with pointer to mesh [list of triangles])
}	t_object;

typedef struct s_aabb
{
	float			pt_min[3];
	float			pt_max[3];
	struct s_aabb	*childs;   // 0 -> left ; 1->right
	t_object		*group;
	int				group_size;
}	t_aabb;

typedef struct s_job
{
	void	(*function)(void *);
	void	*arg;
}	t_job;

typedef struct s_data
{
	t_object			*objects;
	t_light				*lights;
	t_ambient_light		ambient;
	char				*img_buf;
	// first shoot only
	t_camera			cam;
	float				*primary_rays;
	// multithreading
	atomic_int			joblist_size;
	int					joblist_top;
	atomic_int			active_threads;
	t_job				joblist[256];
	pthread_mutex_t		joblist_mutex;
	pthread_t			threads[N_THREAD];
}	t_data;


typedef struct s_shoot
{
	// input
	float			*src;
	float			*dir;
	int				depth;
	// output
	int				res_rgb[3];
	// intermediate
	t_object		*obj;
	float			normal[3];
	float			hit_pt[3];
}	t_shoot;

typedef struct s_calc_img_arg
{
	t_data	*data;
	int		*img;
	int		start;
	int		end;
}	t_calc_img_arg;

typedef struct s_calc_ray_arg
{
	t_data	*data;
	int		start;
	int		end;
}	t_calc_ray_arg;

/// FUNCTIONS


/*parsing.c*/
void	parsing(t_data *data);

/*render.c*/
void	render_first_image(t_data *data, int *img);
void	shoot_ray(t_data *data, t_shoot *shoot);

/*phong.c*/
void		shading(t_shoot *shoot, t_data *data);

/* bvh.c */
t_aabb	*init_bvh(t_object *objects);

/* tests*/
float	intersection_test_sphere(t_sphere *sphere, float p_ray[3], float origin[3]);
float	intersection_test_plane(t_plane *plane, float p_ray[3], float origin[3]);


/*maths*/
float	dot_13_13(float a[3], float b[3]);
void 	normalize(float vector[3]);
void 	normalize2(float vector[3], float *magnitude);
int 	imin(int a, int b);
void	vec_substr(float p1[3], float p2[3], float result[3]);
void	cpy_vec(float v1[3], float v2[3]);

/* Multithreading */
void 	wait_for_workers(t_data *data);
void 	launch_pool(t_data *data);
void	*worker(void *arg);

/* Perf */
void print_render_stats(double render_time);
double stop_timer(clock_t start);
void start_timer(clock_t *start);


#endif
