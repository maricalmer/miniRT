/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:06:35 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/03/26 11:38:19 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTIONS_H
# define FUNCTIONS_H

/// FUNCTIONS
/*prints.c*/
void		print_intro(void);
void		print_outro(void);
void		print_bvh_build_t(struct timeval t_start, struct timeval t_end);
void		print_img_render_t(struct timeval t_start, struct timeval t_end);
void		print_tri_count(int counter);
/*checker.c*/
int			check_input(int ac, char **av, t_data *data);
/*identifiers.c*/
int			is_light(char *specs);
int			is_cam(char *specs);
int			is_ambient(char *specs);
int			is_plane(char *specs);
/*identifiers_2.c*/
int			is_sphere(char *specs);
int			is_cylinder(char *specs);
int			increase_if_uniq(int *value);
/*init.c*/
int			init_rt_lists(t_data *data);
int			init_obj_lists(t_obj_parser *parser);
/*counter.c*/
int			read_rt(t_data *data);
int			read_obj(t_data *data, t_obj_parser *parser);
int			count_rt_elems(char *specs, t_data *data, int *n_cam,
				int *n_ambient);
/*obj_file*/
int			parse_obj_files(t_data *data, char *filename);
int			read_obj_file(t_data *data, t_obj_parser *parser);
int			process_obj_line(char *specs, t_data *data, t_obj_parser *parser);
/*rt_file*/
int			create_elements_rt(t_data *data, char *filename);
/*parser.c*/
int			handle_parsing(char **av, t_data *data);
char		*format_string(char *str, int len);
/*factories.c*/
int			create_ambient_light(t_data *data, char *specs);
int			create_cam(t_data *data, char *specs);
int			create_light(t_data *data, char *specs);
int			create_sphere(t_data *data, char *specs);
int			create_plane(t_data *data, char *specs);
int			create_cylinder(t_data *data, char *specs);
/*factories_utils.c*/
int			get_ratio(char **specs, float *ratio);
int			get_rgb_norm(char **specs, float *color);
int			get_rgb(char **specs, unsigned char *color);
int			get_obj_rgb(char **specs, int *color);
int			get_coord(char **specs, float *value);
int			get_vec_norm(char **specs, float *value);
int			get_fov_range(char **specs, int *fov);
int			get_radius(char **specs, float *radius);
int			get_length(char **specs, float *length);
int			get_pos_float(char **specs, float *size);
/*error.c*/
void		print_error(int errnum);
/*free.c*/
void		free_data(t_data *data);
void		free_obj_parse_1(t_obj_parser *parsers, int n_files);
void		free_obj_parse_2(t_obj_parser *parsers, int n_files);
void		free_at_n_and_v(t_obj_parser *parsers, int n_files);

/*render.c*/
void		render_first_image(t_data *data);
void		calculate_pixel(t_calc_img_arg *arg, int p, t_shoot *shoot,
				int (*hd_res)[3]);
void		shoot_ray(t_data *data, t_shoot *shoot);
void		first_rotation_matrice(t_data *data);
void		calculate_img(t_data *data);
void		calculate_img_packet(t_calc_img_arg *arg);

/*phong.c*/
void		shading(t_shoot *shoot, t_data *data);

/* tests*/
float		visi_tests(t_object *objects, t_shoot *shoot, int n_obj);
float		shadow_tests(t_shoot *shoot, t_object *objects, float dist_light,
				int n_obj);
float		test_sphere(t_object *obj, float ray[3], float origin[3]);
float		test_cylinder(t_object *obj, float ray[3], float origin[3]);
float		test_plane(t_object *obj, float p_ray[3], float origin[3]);

/*maths*/
float		dot_13_13(float a[3], float b[3]);
void		dot_inplace_34_13(double a[3][4], float b[3]);
void		dot_inplace_33_13(float a[3][3], float b[3]);
void		dot_inplace_33_33(float a[3][3], float b[3][3]);
void		dot_inplace_44_44(double a[4][4], double b[4][4]);
void		cprod_13_13(float a[3], float b[3], float res[3]);
void		normalize(float vector[3], float *magnitude);
int			imin(int a, int b);
int			imax(int a, int b);
void		vec_substr(float p1[3], float p2[3], float result[3]);
void		cpy_vec(float v1[3], float v2[3]);
float		triple_scalar(float a[3], float b[3], float c[3]);
void		ft_swap(float *t1, float *t2);
void		get_rotation_matrice(float cam_dir[3], double t_mat[4][4],
				float c[3]);
void		copy_r_mat(t_data *data);
void		copy_r_mat_0(t_data *data);
void		rodrigues_matrice_handler(float u[3], float theta, float c[3],
				double r[4][4]);
void		scale_vec(float v[3], float amp);

/* mlx n events */
int			handle_input(int keysym, t_data *data);
int			handle_close(t_data *data);
int			init_mlx(t_mlxlib *data);
void		rotate_cam(t_data *data, float theta, float axis[3], int anti_fa);
void		translate_cam(t_data *data, float v[3], float amp, int anti_fa);
int			mouse_press(int button, int x, int y, void *arg);
int			mouse_release(int button, int x, int y, void *arg);
int			mouse_move(int x, int y, void *arg);

#endif