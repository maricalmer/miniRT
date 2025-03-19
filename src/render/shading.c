#include "minirt.h"

void	add_phong_diffuse(t_shoot *shoot, t_light light, float theta_LN, unsigned char rgb[3]);
void	add_phong_specular(t_shoot *shoot, t_light light, float theta_LN, unsigned char rgb[3]);
void	add_whitted(t_shoot *shoot, t_data	*data);
void	add_whitted_refraction(t_shoot *shoot, t_data *data);
void	add_whitted_reflection(t_shoot *shoot, t_data *data);
void	shoot_refraction_ray(t_shoot *shoot, t_shoot *new_shoot, t_data *data);
void	shoot_reflection_ray(t_shoot *shoot, t_shoot *new_shoot, t_data *data);
void	calculate_refraction_ray(float p[3], float n[3], float v[3], float r_idx);
float 	get_fresnel(t_shoot *shoot);

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
	float			theta_LN;	
	float			dist_light;
	unsigned char	*rgb;

	if (!shoot->obj)
		return (sky_color(shoot));
	rgb = shoot->obj->mat.rgb;
	if (shoot->obj->mat.checker_flag == 1 && check_checkerboard_grid(shoot))
			rgb = shoot->obj->mat.rgb2;
	add_phong_ambient(shoot, data, rgb);
	i = -1;
	while (++i < data->n_light)
	{
		vec_substr(data->lights[i].origin, shoot->hit_pt, shoot->shadow_ray);
		normalize2(shoot->shadow_ray, &dist_light);
		theta_LN = dot_13_13(shoot->normal, shoot->shadow_ray);
		if (theta_LN > 0 && shadow_tests(shoot, data->objects, dist_light, data->n_obj) < EPSILON)
		{
			add_phong_diffuse(shoot, data->lights[i], theta_LN, rgb);
			add_phong_specular(shoot, data->lights[i], theta_LN, rgb);
		}
	}
	add_whitted(shoot, data);
}
void	add_whitted(t_shoot *shoot, t_data	*data)
{
	if (shoot->depth >= DEPTH_MAX || (shoot->obj->mat.refr_coeff < EPSILON && shoot->obj->mat.refl_coeff < EPSILON))
		return ;
	else if (shoot->obj->mat.refr_coeff >= EPSILON)
		add_whitted_refraction(shoot, data);
	else
		add_whitted_reflection(shoot, data);
}

void	add_whitted_refraction(t_shoot *shoot, t_data *data)
{
	t_shoot			new_shoot_1;
	t_shoot			new_shoot_2;
	int				i;
	float			r;
	
	r = get_fresnel(shoot);
	if (r < FRESNEL_TOLERANCE)
		r = 0;
	if (r > 1 - FRESNEL_TOLERANCE)
		r = 1;
	if (r > 0)
	{
		new_shoot_1.inside = shoot->inside;
		shoot_reflection_ray(shoot, &new_shoot_1, data);
	}
	if (r < 1)
	{
		new_shoot_2.inside = !shoot->inside;
		shoot_refraction_ray(shoot, &new_shoot_2, data);
	}
	i = -1;
	while (++i < 3)
		shoot->res_rgb[i] = (1 - shoot->obj->mat.refr_coeff) * shoot->res_rgb[i]
							+ shoot->obj->mat.refr_coeff * r * new_shoot_1.res_rgb[i]
							+ shoot->obj->mat.refr_coeff * (1 - r) * new_shoot_2.res_rgb[i];
}

void	add_whitted_reflection(t_shoot *shoot, t_data *data)
{
	t_shoot	new_shoot;
	int		i;

	new_shoot.inside = shoot->inside;
	shoot_reflection_ray(shoot, &new_shoot, data);
	i = -1;
	while (++i < 3)
		shoot->res_rgb[i] = (1 - shoot->obj->mat.refl_coeff) * shoot->res_rgb[i]
							+ shoot->obj->mat.refl_coeff * new_shoot.res_rgb[i];
}

float get_fresnel(t_shoot *shoot)
{
	float	n;
	float	n1;
	float	n2;
	float	cos_theta_i;
	float	cos_theta_t;
	float 	r_per;
	float	r_par;

	n1 = 1;
	n2 = shoot->obj->mat.refr_idx;
	if (shoot->inside)
		ft_swap(&n1, &n2);
	cos_theta_i = - dot_13_13(shoot->dir, shoot->normal);
	n = n1 / n2;
	cos_theta_t = 1 - n * n * ( 1 - cos_theta_i * cos_theta_i);
	if (cos_theta_t < 0)
		return 1;
	cos_theta_t = sqrtf(cos_theta_t);
	r_per = (n1 * cos_theta_i - n2 * cos_theta_t) / (n1 * cos_theta_i + n2 * cos_theta_t);
	r_per = r_per * r_per;
	r_par = (n2 * cos_theta_i - n1 * cos_theta_t) / (n2 * cos_theta_i + n1 * cos_theta_t);
	r_par = r_par * r_par;
	return ((r_per + r_par) / 2);
}

void	shoot_reflection_ray(t_shoot *shoot, t_shoot *new_shoot, t_data *data)
{
	float			theta_LN;
	float			bouncing_ray[3];

	theta_LN = dot_13_13(shoot->normal, shoot->dir);
	bouncing_ray[0] = + shoot->dir[0] - 2 * theta_LN * shoot->normal[0];
	bouncing_ray[1] = + shoot->dir[1] - 2 * theta_LN * shoot->normal[1];
	bouncing_ray[2] = + shoot->dir[2] - 2 * theta_LN * shoot->normal[2];
	normalize(bouncing_ray);
	new_shoot->src = shoot->hit_pt;
	cpy_vec(bouncing_ray, new_shoot->dir);
	new_shoot->depth = shoot->depth + 1;
	shoot_ray(data, new_shoot);
}


void	shoot_refraction_ray(t_shoot *shoot, t_shoot *new_shoot, t_data *data)
{
	float	r_entry[3];

	if (shoot->inside)
	{
		new_shoot->inside = 0;
		calculate_refraction_ray(r_entry, shoot->normal, shoot->dir, 1 / shoot->obj->mat.refr_idx);
	}
	else
	{
		new_shoot->inside = 1;
		calculate_refraction_ray(r_entry, shoot->normal, shoot->dir, shoot->obj->mat.refr_idx);
	}
	new_shoot->src = shoot->hit_pt;
	cpy_vec(r_entry, new_shoot->dir);
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

void	add_phong_specular(t_shoot *shoot, t_light light, float theta_LN, unsigned char rgb[3])
{
	int 			i;
	float			reflection_ray[3];
	float			R_dot_E;

	i = -1;
	while (++i < 3)
		reflection_ray[i] = -shoot->shadow_ray[i] + 2 * theta_LN * shoot->normal[i];
	normalize(reflection_ray);
	R_dot_E = - dot_13_13(reflection_ray, shoot->dir);
	R_dot_E = fmaxf(0, R_dot_E);
	R_dot_E = powf(R_dot_E, SPECULAR_POWER);
	i = -1;
	while (++i < 3)
		shoot->res_rgb[i] += light.brightness * R_dot_E * rgb[i] * light.rgb[i]; // phong specular
}
