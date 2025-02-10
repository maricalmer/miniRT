#include "minirt.h"

int		shadow_intersection_tests(t_shoot *shoot, t_data *data, float shadow_ray[3]);
void	add_phong_diffuse(t_shoot *shoot, t_light light, float theta_LN);
void	add_phong_specular(float shadow_ray[3], t_shoot *shoot, t_light light, float theta_LN);

void	shading(t_shoot *shoot, t_data *data)
{
	unsigned char	*rgb;
	int				i;
	float			shadow_ray[3];
	float			theta_LN;	
	float			bouncing_ray[3];


	memset(&shoot->res_rgb, 0, sizeof(shoot->res_rgb));
	if (!shoot->obj)
		{		
			shoot->res_rgb[0] = 70; // move to define header and set ambient color based on 'sky'
			shoot->res_rgb[1] = 130;
			shoot->res_rgb[2] = 180;
			return ;
		}
	rgb = shoot->obj->mat.rgb;
	// ambiant part ==> at the moment white light !
	i = -1;
	while (++i < 3)
		shoot->res_rgb[i] += data->ambient.brightness * rgb[i] * data->ambient.rgb[i] / 255;
	// lights ==> at the moment white light and only one !
	i = 0;
	while (data->lights[i].brightness >= 0)
	{
		vec_substr(shoot->hit_pt, data->lights[i].origin, shadow_ray);
		normalize(shadow_ray);
		if (shadow_intersection_tests(shoot, data, shadow_ray))
		{
			theta_LN = dot_13_13(shoot->normal, shadow_ray);
			add_phong_diffuse(shoot, data->lights[i], theta_LN);
			add_phong_specular(shadow_ray, shoot, data->lights[i], theta_LN);
		}
		i++;
	}


	// whitted reflection : 
	if (shoot->obj->mat.refl_coeff > EPSILON && shoot->depth < DEPTH_MAX)
	{
		t_shoot	new_shoot;

		theta_LN = dot_13_13(shoot->normal, shoot->dir);
		bouncing_ray[0] = + shoot->dir[0] - 2 * theta_LN * shoot->normal[0];
		bouncing_ray[1] = + shoot->dir[1] - 2 * theta_LN * shoot->normal[1];
		bouncing_ray[2] = + shoot->dir[2] - 2 * theta_LN * shoot->normal[2];
		normalize(bouncing_ray);

		new_shoot.src = shoot->hit_pt;
		new_shoot.dir = bouncing_ray;
		new_shoot.depth = shoot->depth + 1;
		shoot_ray(data, &new_shoot);
		i = -1;
		while (++i < 3)
			shoot->res_rgb[i] = (1 - shoot->obj->mat.refl_coeff) * shoot->res_rgb[i]
								+ shoot->obj->mat.refl_coeff * new_shoot.res_rgb[i];
	}

	// whitted refraction : similarlly
		//calculate refraction_ray...
}

void	add_phong_diffuse(t_shoot *shoot, t_light light, float theta_LN)
{
	int				i;
	unsigned char	*rgb;

	rgb = shoot->obj->mat.rgb;
	if (theta_LN > EPSILON)
	{
		i = -1;
		while (++i < 3)
			shoot->res_rgb[i] += light.brightness * theta_LN * rgb[i] * light.rgb[i] / 255; // phong diffuse
	}
}

void	add_phong_specular(float shadow_ray[3], t_shoot *shoot, t_light light, float theta_LN)
{
	int 			i;
	float			reflection_ray[3];
	unsigned char	*rgb;
	float			R_dot_E;

	rgb = shoot->obj->mat.rgb;
	i = -1;
	while (++i < 3)
		reflection_ray[i] = -shadow_ray[i] + 2 * theta_LN * shoot->normal[i];
	normalize(reflection_ray);
	R_dot_E = - dot_13_13(reflection_ray, shoot->dir);
	R_dot_E = fmaxf(0, R_dot_E);
	R_dot_E = powf(R_dot_E, SPECULAR_POWER);
	i = -1;
	while (++i < 3)
		shoot->res_rgb[i] += light.brightness * R_dot_E * rgb[i] * light.rgb[i] / 255; // phong specular
}
/* returns 0 as soon as one object is in the way of light, returns 1 when no shadow*/
int shadow_intersection_tests(t_shoot *shoot, t_data *data, float shadow_ray[3])
{
	t_object	*obj;
	float 		t;

	obj = data->objects;
	while (obj->type)
	{
		if (obj->type == SPHERE)
			t = intersection_test_sphere(obj->geo, shadow_ray, shoot->hit_pt);
		else if (obj->type == PLANE)
		 	t = intersection_test_plane(obj->geo, shadow_ray, shoot->hit_pt);
		if (t > EPSILON)
			return (0);
		obj++;
	}
	return (1);
}