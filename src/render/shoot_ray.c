/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shoot_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:59:58 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/03/23 09:57:59 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	get_hitpt_n_normal_plane(t_shoot *shoot, float t);
static void	get_hitpt_n_normal_sphere(t_shoot *shoot, float t);
static void	get_hitpt_n_normal_tri(t_shoot *shoot);
static void	get_hitpt_n_normal_cyl(t_shoot *shoot, float t);

void	shoot_ray(t_data *data, t_shoot *shoot)
{
	float	t;

	t = visi_tests(data->objects, shoot, data->n_obj);
	if (t > EPSILON)
	{
		if (shoot->obj->type == PLANE)
			get_hitpt_n_normal_plane(shoot, t);
		else if (shoot->obj->type == TRI)
			get_hitpt_n_normal_tri(shoot);
		else if (shoot->obj->type == SPHERE)
			get_hitpt_n_normal_sphere(shoot, t);
		else
			get_hitpt_n_normal_cyl(shoot, t);
		if (dot_13_13(shoot->dir, shoot->normal) > 0)
			scale_vec(shoot->normal, -1);
	}
	else
		shoot->obj = NULL;
	shading(shoot, data);
}

static void	get_hitpt_n_normal_plane(t_shoot *shoot, float t)
{
	shoot->hit_pt[0] = shoot->src[0] + t * shoot->dir[0];
	shoot->hit_pt[1] = shoot->src[1] + t * shoot->dir[1];
	shoot->hit_pt[2] = shoot->src[2] + t * shoot->dir[2];
	shoot->normal[0] = shoot->obj->geo.pl.normal[0];
	shoot->normal[1] = shoot->obj->geo.pl.normal[1];
	shoot->normal[2] = shoot->obj->geo.pl.normal[2];
}

static void	get_hitpt_n_normal_sphere(t_shoot *shoot, float t)
{
	float	r_inv;

	shoot->hit_pt[0] = shoot->src[0] + t * shoot->dir[0];
	shoot->hit_pt[1] = shoot->src[1] + t * shoot->dir[1];
	shoot->hit_pt[2] = shoot->src[2] + t * shoot->dir[2];
	r_inv = 1 / shoot->obj->geo.sph.radius;
	shoot->normal[0] = r_inv * (shoot->hit_pt[0]
			- shoot->obj->geo.sph.center[0]);
	shoot->normal[1] = r_inv * (shoot->hit_pt[1]
			- shoot->obj->geo.sph.center[1]);
	shoot->normal[2] = r_inv * (shoot->hit_pt[2]
			- shoot->obj->geo.sph.center[2]);
}

static void	get_hitpt_n_normal_tri(t_shoot *shoot)
{
	float	px[3][3];
	float	bary[3];
	float	denom;
	int		i;

	vec_substr(shoot->obj->geo.tri.v0, shoot->src, px[0]);
	vec_substr(shoot->obj->geo.tri.v1, shoot->src, px[1]);
	vec_substr(shoot->obj->geo.tri.v2, shoot->src, px[2]);
	bary[0] = triple_scalar(shoot->dir, px[2], px[1]);
	bary[1] = triple_scalar(shoot->dir, px[0], px[2]);
	bary[2] = triple_scalar(shoot->dir, px[1], px[0]);
	denom = 1.0 / (bary[0] + bary[1] + bary[2]);
	i = -1;
	while (++i < 3)
		bary[i] *= denom;
	i = -1;
	while (++i < 3)
	{
		shoot->hit_pt[i] = bary[0] * shoot->obj->geo.tri.v0[i]
			+ bary[1] * shoot->obj->geo.tri.v1[i]
			+ bary[2] * shoot->obj->geo.tri.v2[i];
		shoot->normal[i] = shoot->obj->geo.tri.n0[i] * bary[0]
			+ shoot->obj->geo.tri.n1[i] * bary[1]
			+ shoot->obj->geo.tri.n2[i] * bary[2];
	}
}

static void	get_hitpt_n_normal_cyl(t_shoot *shoot, float t)
{
	int		i;
	float	tmp[3];
	float	r_inv;
	float	h;

	shoot->hit_pt[0] = shoot->src[0] + t * shoot->dir[0];
	shoot->hit_pt[1] = shoot->src[1] + t * shoot->dir[1];
	shoot->hit_pt[2] = shoot->src[2] + t * shoot->dir[2];
	vec_substr(shoot->hit_pt, shoot->obj->geo.cyl.center, tmp);
	h = dot_13_13(tmp, shoot->obj->geo.cyl.dir);
	i = -1;
	while (++i < 3)
		tmp[i] = (shoot->obj->geo.cyl.center[i] + h)
			* shoot->obj->geo.cyl.dir[i];
	r_inv = 1 / shoot->obj->geo.cyl.radius;
	shoot->normal[0] = r_inv * (shoot->hit_pt[0] - tmp[0]);
	shoot->normal[1] = r_inv * (shoot->hit_pt[1] - tmp[1]);
	shoot->normal[2] = r_inv * (shoot->hit_pt[2] - tmp[2]);
}
