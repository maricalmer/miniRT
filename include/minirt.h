#ifndef MINIRT_H
# define MINIRT_H

# include "libft.h"
# include "mlx.h"
# include <X11/keysym.h>
# include <X11/X.h>
# include <math.h>
# include <float.h>

# define WIDTH		6
# define HEIGHT		6
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
}	t_intersect;

typedef struct s_data
{
	t_camera	cam;
	t_object	*objects;
	/*float		*primary_rays;*/
	float		primary_rays[HEIGHT][WIDTH][3];
//	
}	t_data;



/// FUNCTIONS


/*parsing.c*/
void	parsing(t_data *data);
void	render_first_image(t_data *data);

/*parsing.c*/
void	parsing(t_data *data);
#endif
