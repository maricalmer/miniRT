/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shoot_refrac_reflec.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:29:21 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 15:59:40 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Handles reflection and refraction ray generation for Whitted algo.         */
/* Computes the direction of bounced (mirror) rays and transmitted            */
/* (refracted) rays. Uses surface normals and material properties for         */
/* calculation. Reflection preserves the current medium, refraction toggles   */
/* the inside/outside state. New rays are recursively traced with increased   */
/* depth for multi-bounce effects. Central for simulating glass, mirrors and  */
/* other transparent or shiny surfaces.                                       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	calculate_refraction_ray(float p[3], float n[3],
				float v[3], float r_idx);

void	shoot_reflection_ray(t_shoot *shoot, t_shoot *new_shoot, t_data *data)
{
	float			theta_ln;
	float			bouncing_ray[3];

	theta_ln = dot_vec3(shoot->normal, shoot->dir);
	bouncing_ray[0] = shoot->dir[0] - 2 * theta_ln * shoot->normal[0];
	bouncing_ray[1] = shoot->dir[1] - 2 * theta_ln * shoot->normal[1];
	bouncing_ray[2] = shoot->dir[2] - 2 * theta_ln * shoot->normal[2];
	normalize(bouncing_ray, NULL);
	new_shoot->src = shoot->hit_pt;
	copy_vec(bouncing_ray, new_shoot->dir);
	new_shoot->depth = shoot->depth + 1;
	new_shoot->inside = shoot->inside;
	shoot_ray(data, new_shoot);
}

void	shoot_refraction_ray(t_shoot *shoot, t_shoot *new_shoot, t_data *data)
{
	float	r_entry[3];

	if (shoot->inside)
		calculate_refraction_ray(r_entry, shoot->normal,
			shoot->dir, 1 / shoot->obj->mat.refr_idx);
	else
		calculate_refraction_ray(r_entry, shoot->normal,
			shoot->dir, shoot->obj->mat.refr_idx);
	new_shoot->src = shoot->hit_pt;
	copy_vec(r_entry, new_shoot->dir);
	new_shoot->depth = shoot->depth + 1;
	new_shoot->inside = !shoot->inside;
	shoot_ray(data, new_shoot);
}

static void	calculate_refraction_ray(float p[3], float n[3],
				float v[3], float r_idx)
{
	float	vp[3];
	float	v_dot_n_inv;
	int		i;
	float	kf;

	v_dot_n_inv = 1 / (-dot_vec3(v, n));
	i = -1;
	while (++i < 3)
		vp[i] = v[i] * v_dot_n_inv;
	kf = r_idx * r_idx * (vp[0] * vp[0] + vp[1] * vp[1] + vp[2] * vp[2]);
	kf -= (vp[0] + n[0]) * (vp[0] + n[0]) + (vp[1] + n[1]) * (vp[1] + n[1])
		+ (vp[2] + n[2]) * (vp[2] + n[2]);
	kf = sqrtf(kf);
	kf = 1 / kf;
	i = -1;
	while (++i < 3)
		p[i] = kf * (n[i] + vp[i]) - n[i];
	normalize(p, NULL);
}
