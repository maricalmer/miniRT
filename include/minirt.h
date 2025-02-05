#ifndef MINIRT_H
# define MINIRT_H

# include "libft.h"
# include "mlx.h"
# include <X11/keysym.h>
# include <X11/X.h>
# include <math.h>
# include <float.h>
# include <stdio.h>

# define WIDTH		600
# define HEIGHT		600
# define EPSILON    0.001 // adjust

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

typedef struct s_object
{
	t_obj_type	type;
	
	void		*data;
}	t_object;

typedef struct s_sphere
{
	float			center[3];
	float			radius;
	unsigned char	rgb[3];
}	t_sphere;

typedef struct s_plane
{
	float			point[3];
	float			normal[3];
	unsigned char	rgb[3];
}	t_plane;

typedef struct s_intersect
{
	t_object		*obj;
	float			hit_pt[3];
	float			normal[3];
}	t_intersect;

typedef struct s_data
{
	t_camera			cam;
	t_object			*objects;
	t_light				*lights;
	t_ambient_light		ambient;
	float				primary_rays[HEIGHT][WIDTH][3];
	/*float		*primary_rays;*/
//	
}	t_data;



/// FUNCTIONS


/*parsing.c*/
void	parsing(t_data *data);
void	render_first_image(t_data *data, int *img);

/*phong.c*/
int		phong(t_intersect *first, t_data *data);


/* tests*/
float	intersection_test_sphere(t_sphere *sphere, float p_ray[3], float origin[3]);
float	intersection_test_plane(t_plane *plane, float p_ray[3], float origin[3]);


/*maths*/
float	dot_13_13(float a[3], float b[3]);
void 	normalize(float vector[3]);
#endif
