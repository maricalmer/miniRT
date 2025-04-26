/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:32:55 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 14:18:25 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Error handling and reporting functions. Error codes are interpreted and    */
/* translated into descriptive, color-formatted messages.                     */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	print_scene_element_error(int error_code);

void	handle_memory_failure(const char *function)
{
	printf("\n%sError at %s%s%s%s%s\n", RED_BG_START, function, COLOR_END,
		RED_TXT_START, strerror(errno), COLOR_END);
	exit(EXIT_FAILURE);
}

void	handle_file_error(const char *function, const char *filename)
{
	printf("\n%s%s: Failed to open %s%s%s%s%s\n", RED_BG_START, function,
		filename, COLOR_END, RED_TXT_START, strerror(errno), COLOR_END);
	exit(EXIT_FAILURE);
}

void	print_error(int error_code)
{
	if (error_code == ARGS_ERROR)
		printf("\n%sError%s%sWrong args%s\n", RED_BG_START, COLOR_END,
			RED_TXT_START, COLOR_END);
	else if (error_code == FILE_ERROR)
		printf("\n%sError%s%sWrong input file%s\n", RED_BG_START, COLOR_END,
			RED_TXT_START, COLOR_END);
	else if (error_code == MAND_SET_ERROR)
		printf("\n%sError%s%sWrong set of light, cam or ambient%s\n",
			RED_BG_START, COLOR_END, RED_TXT_START, COLOR_END);
	else if (error_code == TYPES_ERROR)
		printf("\n%sError%s%sWrong set of element types%s\n", RED_BG_START,
			COLOR_END, RED_TXT_START, COLOR_END);
	else if (error_code == AMB_ERROR)
		printf("\n%sError%s%sWrong settings for ambient lighting element%s\n",
			RED_BG_START, COLOR_END, RED_TXT_START, COLOR_END);
	else if (error_code == CAM_ERROR)
		printf("\n%sError%s%sWrong settings for camera element%s\n",
			RED_BG_START, COLOR_END, RED_TXT_START, COLOR_END);
	else
		print_scene_element_error(error_code);
}

static void	print_scene_element_error(int error_code)
{
	if (error_code == LIGHT_ERROR)
		printf("\n%sError%s%sWrong settings for light element%s\n",
			RED_BG_START, COLOR_END, RED_TXT_START, COLOR_END);
	else if (error_code == SPHERE_ERROR)
		printf("\n%sError%s%sWrong settings for sphere element%s\n",
			RED_BG_START, COLOR_END, RED_TXT_START, COLOR_END);
	else if (error_code == PLANE_ERROR)
		printf("\n%sError%s%sWrong settings for plane element%s\n",
			RED_BG_START, COLOR_END, RED_TXT_START, COLOR_END);
	else if (error_code == CYLINDER_ERROR)
		printf("\n%sError%s%sWrong settings for cylinder element%s\n",
			RED_BG_START, COLOR_END, RED_TXT_START, COLOR_END);
	else if (error_code == RECTANGLE_ERROR)
		printf("\n%sError%s%sWrong settings for rectangle element%s\n",
			RED_BG_START, COLOR_END, RED_TXT_START, COLOR_END);
	else if (error_code == TRI_ERROR)
		printf("\n%sError%s%sWrong settings for tri element%s\n", RED_BG_START,
			COLOR_END, RED_TXT_START, COLOR_END);
	else if (error_code == OBJ_ERROR)
		printf("\n%sError%s%sWrong settings for obj element%s\n", RED_BG_START,
			COLOR_END, RED_TXT_START, COLOR_END);
}
