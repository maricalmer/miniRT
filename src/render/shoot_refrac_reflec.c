/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shoot_refrac_reflec.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:29:21 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 17:25:45 by dlemaire         ###   ########.fr       */
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

static void	compute_refracted_ray(float p[3], float n[3],
				float v[3], float r_idx);

void	shoot_reflection_ray(t_shoot *shoot, t_shoot *new_shoot, t_data *data)
{
	float			theta_ln;
	float			reflected_ray[3];

	theta_ln = dot_vec3(shoot->normal, shoot->dir);
	reflected_ray[0] = shoot->dir[0] - 2 * theta_ln * shoot->normal[0];
	reflected_ray[1] = shoot->dir[1] - 2 * theta_ln * shoot->normal[1];
	reflected_ray[2] = shoot->dir[2] - 2 * theta_ln * shoot->normal[2];
	normalize(reflected_ray, NULL);
	new_shoot->src = shoot->hit_pt;
	copy_vec(reflected_ray, new_shoot->dir);
	new_shoot->depth = shoot->depth + 1;
	new_shoot->inside = shoot->inside;
	shoot_ray(data, new_shoot);
}

void	shoot_refraction_ray(t_shoot *shoot, t_shoot *new_shoot, t_data *data)
{
	float	refracted_ray[3];

	if (shoot->inside)
		compute_refracted_ray(refracted_ray, shoot->normal,
			shoot->dir, 1 / shoot->obj->mat.refr_idx);
	else
		compute_refracted_ray(refracted_ray, shoot->normal,
			shoot->dir, shoot->obj->mat.refr_idx);
	new_shoot->src = shoot->hit_pt;
	copy_vec(refracted_ray, new_shoot->dir);
	new_shoot->depth = shoot->depth + 1;
	new_shoot->inside = !shoot->inside;
	shoot_ray(data, new_shoot);
}

static void	compute_refracted_ray(float p[3], float n[3],
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
