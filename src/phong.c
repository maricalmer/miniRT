#include "minirt.h"

void	add_phong_diffuse(t_shoot *shoot, t_light light, float theta_LN);
void	add_phong_specular(float shadow_ray[3], t_shoot *shoot, t_light light, float theta_LN);

void	shading(t_shoot *shoot, t_data *data)
{
	unsigned char	*rgb;
	int				i;
	float			shadow_ray[3];
	float			theta_LN;	
	float			bouncing_ray[3];
	float			dist_light;

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
		shoot->res_rgb[i] += data->ambient.brightness * rgb[i] * data->ambient.rgb[i];
	// lights ==> at the moment white light and only one !
	i = 0;
	while (data->lights[i].brightness >= 0)
	{
		vec_substr(shoot->hit_pt, data->lights[i].origin, shadow_ray);
		normalize2(shadow_ray, &dist_light);
		if (shadow_intersection_tests(shoot, data->objects, shadow_ray, dist_light, data->n_obj) < EPSILON)
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
	theta_LN = fmaxf(theta_LN , -theta_LN);
	if (1)//theta_LN > EPSILON)
	{
		i = -1;
		while (++i < 3)
			shoot->res_rgb[i] += light.brightness * theta_LN * rgb[i] * light.rgb[i]; // phong diffuse
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
		shoot->res_rgb[i] += light.brightness * R_dot_E * rgb[i] * light.rgb[i]; // phong specular
}
