#include "minirt.h"

void	add_phong_diffuse(t_shoot *shoot, t_light light, float theta_LN, unsigned char rgb[3]);
void	add_phong_specular(float shadow_ray[3], t_shoot *shoot, t_light light, float theta_LN, unsigned char rgb[3]);
void	add_whitted(t_shoot *shoot, t_data	*data);
void	shoot_refraction_ray(t_shoot *shoot, t_shoot *new_shoot, t_data *data);
void	calculate_refraction_ray(float p[3], float n[3], float v[3], float r_idx);

void	sky_color(t_shoot *shoot)
{		
	shoot->res_rgb[0] = SKY_COLOR_R;
	shoot->res_rgb[1] = SKY_COLOR_G;
	shoot->res_rgb[2] = SKY_COLOR_B;
	return ;
}
void	add_phong_ambient(t_shoot *shoot, t_data *data, unsigned char rgb[3])
{
	int	i;
	
	i = -1;
	while (++i < 3)
		shoot->res_rgb[i] = data->ambient.brightness * rgb[i] * data->ambient.rgb[i];
}


void	shading(t_shoot *shoot, t_data *data)
{
	int				i;
	float			shadow_ray[3];
	float			theta_LN;	
	float			dist_light;
	unsigned char	*rgb;



	if (!shoot->obj)
		return (sky_color(shoot));
	rgb = shoot->obj->mat.rgb;
	if (shoot->obj->mat.checker_flag == 1)
	{
		if (check_checkerboard_grid(shoot))
			rgb = shoot->obj->mat.rgb2;
	}
	add_phong_ambient(shoot, data, rgb);
	i = -1;
	while (data->lights[++i].brightness >= 0)
	{
		vec_substr(data->lights[i].origin, shoot->hit_pt, shadow_ray);
		normalize2(shadow_ray, &dist_light);
		theta_LN = dot_13_13(shoot->normal, shadow_ray);
		if (theta_LN > EPSILON && shadow_intersection_tests(shoot, data->objects, shadow_ray, dist_light, data->n_obj) == 0)
		{
			add_phong_diffuse(shoot, data->lights[i], theta_LN, rgb);
			add_phong_specular(shadow_ray, shoot, data->lights[i], theta_LN, rgb);
		}
	}
	add_whitted(shoot, data);
}
void	add_whitted(t_shoot *shoot, t_data	*data)
{
	float			bouncing_ray[3];
	float			theta_LN;
	t_shoot			new_shoot_1;
	t_shoot			new_shoot_2;
	int				i;

	if (shoot->depth < DEPTH_MAX && (shoot->obj->mat.refr_coeff > EPSILON || shoot->obj->mat.refl_coeff > EPSILON))
	{
		//ft_memset(&new_shoot_1, 0, sizeof(t_shoot));
		//ft_memset(&new_shoot_2, 0, sizeof(t_shoot));
		// reflection :
		if (shoot->obj->mat.refl_coeff > EPSILON )
		{
			theta_LN = dot_13_13(shoot->normal, shoot->dir);
			bouncing_ray[0] = + shoot->dir[0] - 2 * theta_LN * shoot->normal[0];
			bouncing_ray[1] = + shoot->dir[1] - 2 * theta_LN * shoot->normal[1];
			bouncing_ray[2] = + shoot->dir[2] - 2 * theta_LN * shoot->normal[2];
			normalize(bouncing_ray);
			new_shoot_1.src = shoot->hit_pt;
			cpy_vec(bouncing_ray, new_shoot_1.dir);
			new_shoot_1.depth = shoot->depth + 1;
			shoot_ray(data, &new_shoot_1);
		}
		// refraction
		if (shoot->obj->mat.refr_coeff > EPSILON )
			shoot_refraction_ray(shoot, &new_shoot_2, data);

		// add all values together;
		i = -1;
		while (++i < 3)
			shoot->res_rgb[i] = (1 - shoot->obj->mat.refl_coeff - shoot->obj->mat.refr_coeff) * shoot->res_rgb[i]
								+ shoot->obj->mat.refl_coeff * new_shoot_1.res_rgb[i]
								+ shoot->obj->mat.refr_coeff * new_shoot_2.res_rgb[i];
	}
}

void	shoot_refraction_ray(t_shoot *shoot, t_shoot *new_shoot, t_data *data)
{
	float	r_entry[3];
	float	n_exit[3];
	float	t;
	int		i;
	float	r_inv;
	float	p_exit[3];
	float	r_exit[3];


	calculate_refraction_ray(r_entry, shoot->normal, shoot->dir, shoot->obj->mat.refr_idx);
	t =  intersection_test_sphere2(shoot->obj, r_entry, shoot->hit_pt);
	i = -1;
	while (++i < 3)
		p_exit[i] = shoot->hit_pt[i] + t * r_entry[i];
	//get_exit_normal(&n_exit, shoot->obj->geo, new_shoot->src);
	r_inv = 1 / shoot->obj->geo.sph.radius;
	n_exit[0] = r_inv * (-p_exit[0] + shoot->obj->geo.sph.center[0]);
	n_exit[1] = r_inv * (-p_exit[1] + shoot->obj->geo.sph.center[1]);
	n_exit[2] = r_inv * (-p_exit[2] + shoot->obj->geo.sph.center[2]);
	//
	
	calculate_refraction_ray(r_exit, n_exit, r_entry, 1 / shoot->obj->mat.refr_idx);
	new_shoot->src = p_exit;
	cpy_vec(r_exit, new_shoot->dir);
	new_shoot->depth = shoot->depth + 1;
	shoot_ray(data, new_shoot);
}

void	calculate_refraction_ray(float p[3], float n[3], float v[3], float r_idx)
{
	float	vp[3];
	float	v_dot_n_inv;
	int		i;
	float	kf;

	v_dot_n_inv = 1 / (-dot_13_13(v, n));
	i = -1;
	while (++i < 3)
		vp[i] = v[i] * v_dot_n_inv;
	kf = r_idx * r_idx * (vp[0] * vp[0] + vp[1] * vp[1] + vp[2] * vp[2]);
	kf -= (vp[0] + n[0]) * (vp[0] + n[0]) + (vp[1] + n[1]) * (vp[1] + n[1]) + (vp[2] + n[2]) * (vp[2] + n[2]);
	// if (kf < EPSILON)
	// 	write(1, "dammed\n", 7);
	kf = sqrtf(kf);
	kf = 1 / kf;
	i = -1;
	while (++i < 3)
		p[i] = kf * (n[i] + vp[i]) - n[i];
	normalize(p);
}
void	calculate_refraction_ray_exit(float p[3], float n[3], float v[3], float r_idx)
{
	float	vp[3];
	float	v_dot_n_inv;
	int		i;
	float	kf;

	v_dot_n_inv = 1 / (-dot_13_13(v, n));
	i = -1;
	while (++i < 3)
		vp[i] = v[i] * v_dot_n_inv;
	kf = r_idx * r_idx * (vp[0] * vp[0] + vp[1] * vp[1] + vp[2] * vp[2]);
	kf -= (vp[0] + n[0]) * (vp[0] + n[0]) + (vp[1] + n[1]) * (vp[1] + n[1]) + (vp[2] + n[2]) * (vp[2] + n[2]);
	// if (kf < EPSILON)
	// 	write(1, "dammed\n", 7);
	kf = sqrtf(kf);
	kf = 1 / kf;
	i = -1;
	while (++i < 3)
		p[i] = kf * (n[i] + vp[i]) - n[i];
	normalize(p);
}

void	add_phong_diffuse(t_shoot *shoot, t_light light, float theta_LN, unsigned char rgb[3])
{
	int				i;

	i = -1;
	while (++i < 3)
			shoot->res_rgb[i] += light.brightness * theta_LN * rgb[i] * light.rgb[i];

}

void	add_phong_specular(float shadow_ray[3], t_shoot *shoot, t_light light, float theta_LN, unsigned char rgb[3])
{
	int 			i;
	float			reflection_ray[3];
	float			R_dot_E;

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
