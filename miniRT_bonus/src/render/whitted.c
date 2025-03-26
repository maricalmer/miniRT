/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whitted.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:23:09 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/03/24 16:25:18 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void		add_whitted_refraction(t_shoot *shoot, t_data *data);
static void		add_whitted_reflection(t_shoot *shoot, t_data *data);
static float	get_fresnel(t_shoot *shoot);

void			shoot_refraction_ray(t_shoot *shoot, t_shoot *new_shoot,
					t_data *data);
void			shoot_reflection_ray(t_shoot *shoot, t_shoot *new_shoot,
					t_data *data);

void	add_whitted(t_shoot *shoot, t_data	*data)
{
	if (shoot->depth >= DEPTH_MAX || (shoot->obj->mat.refr_coeff < EPSILON
			&& shoot->obj->mat.refl_coeff < EPSILON)
		|| shoot->intens < FRESNEL_TOLERANCE)
		return ;
	else if (shoot->obj->mat.refr_coeff >= EPSILON)
		add_whitted_refraction(shoot, data);
	else
		add_whitted_reflection(shoot, data);
}

static void	add_whitted_refraction(t_shoot *shoot, t_data *data)
{
	t_shoot			new_shoot_1;
	t_shoot			new_shoot_2;
	int				i;
	float			r;

	r = get_fresnel(shoot);
	new_shoot_1.intens = shoot->intens * shoot->obj->mat.refr_coeff * r;
	new_shoot_2.intens = shoot->intens * shoot->obj->mat.refr_coeff * (1 - r);
	shoot_reflection_ray(shoot, &new_shoot_1, data);
	shoot_refraction_ray(shoot, &new_shoot_2, data);
	i = -1;
	while (++i < 3)
		shoot->res_rgb[i] = (1 - shoot->obj->mat.refr_coeff) * shoot->res_rgb[i]
			+ shoot->obj->mat.refr_coeff * r * new_shoot_1.res_rgb[i]
			+ shoot->obj->mat.refr_coeff * (1 - r) * new_shoot_2.res_rgb[i];
}

static void	add_whitted_reflection(t_shoot *shoot, t_data *data)
{
	t_shoot	new_shoot;
	int		i;

	new_shoot.intens = shoot->intens * shoot->obj->mat.refl_coeff;
	shoot_reflection_ray(shoot, &new_shoot, data);
	i = -1;
	while (++i < 3)
		shoot->res_rgb[i] = (1 - shoot->obj->mat.refl_coeff) * shoot->res_rgb[i]
			+ shoot->obj->mat.refl_coeff * new_shoot.res_rgb[i];
}

static float	get_fresnel(t_shoot *shoot)
{
	float	n[2];
	float	cos_theta_i;
	float	cos_theta_t;
	float	r_per;
	float	r_par;

	n[0] = 1;
	n[1] = shoot->obj->mat.refr_idx;
	if (shoot->inside)
		ft_swap(&n[0], &n[1]);
	cos_theta_i = -dot_13_13(shoot->dir, shoot->normal);
	cos_theta_t = 1 - n[0] * n[0] / n[1] / n[1]
		* (1 - cos_theta_i * cos_theta_i);
	if (cos_theta_t < 0)
		return (1);
	cos_theta_t = sqrtf(cos_theta_t);
	r_per = (n[0] * cos_theta_i - n[1] * cos_theta_t)
		/ (n[0] * cos_theta_i + n[1] * cos_theta_t);
	r_per = r_per * r_per;
	r_par = (n[1] * cos_theta_i - n[0] * cos_theta_t)
		/ (n[1] * cos_theta_i + n[0] * cos_theta_t);
	r_par = r_par * r_par;
	return ((r_per + r_par) / 2);
}
