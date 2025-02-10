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

# define WIDTH				1200
# define HEIGHT				900
# define EPSILON    		0.001 // adjust
# define SPECULAR_POWER 	50
# define DEPTH_MAX			20
# define ANTIALIASING_FACT	1

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
	unsigned char	rgb[3];
}	t_light;

typedef struct s_ambient_light
{
	float			brightness;
	unsigned char	rgb[3];
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

typedef struct s_data
{
	t_object			*objects;
	t_light				*lights;
	t_ambient_light		ambient;
	// first shoot only
	t_camera			cam;
	float				*primary_rays;	
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




/// FUNCTIONS


/*parsing.c*/
void	parsing(t_data *data);

/*render.c*/
void	render_first_image(t_data *data, int *img);
void	shoot_ray(t_data *data, t_shoot *shoot);

/*phong.c*/
void		shading(t_shoot *shoot, t_data *data);


/* tests*/
float	intersection_test_sphere(t_sphere *sphere, float p_ray[3], float origin[3]);
float	intersection_test_plane(t_plane *plane, float p_ray[3], float origin[3]);


/*maths*/
float	dot_13_13(float a[3], float b[3]);
void 	normalize(float vector[3]);
int 	imin(int a, int b);
#endif
