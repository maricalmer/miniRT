/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:10:51 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/03/19 21:52:36 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	sky_color(t_shoot *shoot);
static void	add_phong_ambient(t_shoot *shoot, t_data *data,
				unsigned char rgb[3]);
static void	add_phong_diffuse(t_shoot *shoot, t_light light, float theta_LN,
				unsigned char rgb[3]);
static void	add_phong_specular(t_shoot *shoot, t_light light, float theta_LN,
				unsigned char rgb[3]);
void		add_whitted(t_shoot *shoot, t_data	*data);

void	shading(t_shoot *shoot, t_data *data)
{
	int				i;
	float			theta_ln;	
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
		theta_ln = dot_13_13(shoot->normal, shoot->shadow_ray);
		if (theta_ln > 0 && shadow_tests
			(shoot, data->objects, dist_light, data->n_obj) < EPSILON)
		{
			add_phong_diffuse(shoot, data->lights[i], theta_ln, rgb);
			add_phong_specular(shoot, data->lights[i], theta_ln, rgb);
		}
	}
	add_whitted(shoot, data);
}

static void	add_phong_ambient(t_shoot *shoot, t_data *data,
				unsigned char rgb[3])
{
	int	i;

	i = -1;
	while (++i < 3)
		shoot->res_rgb[i] = data->ambient.brightness * rgb[i]
			* data->ambient.rgb[i];
}

static void	add_phong_diffuse(t_shoot *shoot, t_light light, float theta_LN,
				unsigned char rgb[3])
{
	int				i;

	i = -1;
	while (++i < 3)
		shoot->res_rgb[i] += light.brightness * theta_LN * rgb[i]
			* light.rgb[i];
}

static void	add_phong_specular(t_shoot *shoot, t_light light, float theta_LN,
				unsigned char rgb[3])
{
	int		i;
	float	reflection_ray[3];
	float	r_dot_e;

	i = -1;
	while (++i < 3)
		reflection_ray[i] = -shoot->shadow_ray[i]
			+ 2 * theta_LN * shoot->normal[i];
	normalize(reflection_ray);
	r_dot_e = -dot_13_13(reflection_ray, shoot->dir);
	r_dot_e = fmaxf(0, r_dot_e);
	r_dot_e = powf(r_dot_e, SPECULAR_POWER);
	i = -1;
	while (++i < 3)
		shoot->res_rgb[i] += light.brightness * r_dot_e * rgb[i] * light.rgb[i];
}

static void	sky_color(t_shoot *shoot)
{
	shoot->res_rgb[0] = SKY_COLOR_R;
	shoot->res_rgb[1] = SKY_COLOR_G;
	shoot->res_rgb[2] = SKY_COLOR_B;
	return ;
}
