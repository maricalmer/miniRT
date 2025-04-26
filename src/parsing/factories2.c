/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factories2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:49:04 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 16:36:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Creation of sphere and triangle objects. It parses core parameters like    */
/* position, color and size. For triangles, it validates face data and        */
/* extracts vertex and normal vectors.                                        */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	is_valid_triangle_face(char *line, int v[3], int vn[3], int vt[3]);
static void	set_tri_n_and_v(t_data *data, t_obj_parser *parser,
				int v[3], int vn[3]);
static void	set_tri_color(t_data *data, t_obj_parser *parser);

int	create_sphere(t_data *data, char *specs)
{
	if (get_coord(&specs, data->objects[data->objects_idx].geo.sph.center)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_radius(&specs, &data->objects[data->objects_idx].geo.sph.radius)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_rgb(&specs, data->objects[data->objects_idx].mat.rgb)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->objects[data->objects_idx].type = SPHERE;
	data->objects[data->objects_idx].mat.checker_size = 0;
	if (get_ratio(&specs, &data->objects[data->objects_idx].mat.refl_coeff)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_ratio(&specs, &data->objects[data->objects_idx].mat.refr_coeff)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_refraction_index(&specs, &data->objects[data->objects_idx].mat.refr_idx)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	create_triangle(t_data *data, char *line, t_obj_parser *parser)
{
	int	v[3];
	int	vn[3];
	int	vt[3];

	if (is_valid_triangle_face(line, v, vn, vt))
	{
		set_tri_n_and_v(data, parser, v, vn);
		data->objects[data->objects_idx].type = TRI;
		data->objects[data->objects_idx].mat.checker_size = 0;
		data->objects[data->objects_idx].mat.refl_coeff 
			= parser->tri_refl_coeff;
		data->objects[data->objects_idx].mat.refr_coeff 
			= parser->tri_refr_coeff;
		data->objects[data->objects_idx].mat.refr_idx = parser->tri_refr_idx;
		set_tri_color(data, parser);
		data->objects_idx++;
		return (EXIT_SUCCESS);
	}
	else
		return (EXIT_FAILURE);
}

static int	is_valid_triangle_face(char *line, int v[3], int vn[3], int vt[3])
{
	if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
			&v[0], &vt[0], &vn[0], 
			&v[1], &vt[1], &vn[1], 
			&v[2], &vt[2], &vn[2]) == 9)
		return (1);
	if (sscanf(line, "f %d//%d %d//%d %d//%d", 
			&v[0], &vn[0], 
			&v[1], &vn[1], 
			&v[2], &vn[2]) == 6)
		return (1);
	return (0);
}

static void	set_tri_n_and_v(t_data *data, t_obj_parser *parser,
	int v[3], int vn[3])
{
	int	i;

	i = -1;
	while (++i < 3)
	{
		v[i]--;
		vn[i]--;
	}
	i = -1;
	while (++i < 3)
	{
		data->objects[data->objects_idx].geo.tri.v0[i] 
			= parser->vertices[v[0]][i];
		data->objects[data->objects_idx].geo.tri.v1[i] 
			= parser->vertices[v[1]][i];
		data->objects[data->objects_idx].geo.tri.v2[i] 
			= parser->vertices[v[2]][i];
		data->objects[data->objects_idx].geo.tri.n0[i] 
			= parser->normals[vn[0]][i];
		data->objects[data->objects_idx].geo.tri.n1[i] 
			= parser->normals[vn[1]][i];
		data->objects[data->objects_idx].geo.tri.n2[i] 
			= parser->normals[vn[2]][i];
	}
}

static void	set_tri_color(t_data *data, t_obj_parser *parser)
{
	int	i;

	i = -1;
	while (++i < 3)
	{
		if (parser->tri_rgb[i] == -1)
			data->objects[data->objects_idx].mat.rgb[i] = rand() % 256;
		else
			data->objects[data->objects_idx].mat.rgb[i] = parser->tri_rgb[i];
	}
}
