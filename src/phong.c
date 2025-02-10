#include "minirt.h"

int shadow_intersection_tests(t_shoot *shoot, t_data *data, float shadow_ray[3]);

void	shading(t_shoot *shoot, t_data *data)
{
	unsigned char	*rgb;
	int				i;
	float			theta_LN;
	float			shadow_ray[3];
	float			reflection_ray[3];
	float			bouncing_ray[3];
	float			R_dot_E;

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
		shoot->res_rgb[i] += data->ambient.brightness * rgb[i]; // pr color lights --> *light_color[i]/255
	// lights ==> at the moment white light and only one !
	shadow_ray[0] = data->lights[0].origin[0] - shoot->hit_pt[0];
	shadow_ray[1] = data->lights[0].origin[1] - shoot->hit_pt[1];
	shadow_ray[2] = data->lights[0].origin[2] - shoot->hit_pt[2];
	normalize(shadow_ray);
	theta_LN = dot_13_13(shoot->normal, shadow_ray);
	int in_light = shadow_intersection_tests(shoot, data, shadow_ray);
	// diffuse light ==> do for each spot light
	if (theta_LN > EPSILON && in_light)
	{
		i = -1;
	 	while (++i < 3)
			shoot->res_rgb[i] += data->lights[0].brightness * theta_LN * rgb[i]; // phong diffuse
	}
	// specular spot ==> do for each spot light
	reflection_ray[0] = -shadow_ray[0] + 2 * theta_LN * shoot->normal[0];
	reflection_ray[1] = -shadow_ray[1] + 2 * theta_LN * shoot->normal[1];
	reflection_ray[2] = -shadow_ray[2] + 2 * theta_LN * shoot->normal[2];
	normalize(reflection_ray);
	R_dot_E = - dot_13_13(reflection_ray, shoot->dir);
	R_dot_E = fmaxf(0, R_dot_E);
	R_dot_E = powf(R_dot_E, SPECULAR_POWER);
	if (in_light)
	{
		i = -1;
		while (++i < 3)
			shoot->res_rgb[i] += data->lights[0].brightness * R_dot_E * rgb[i]; // phong specular
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