/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototypes.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:49:20 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/26 17:05:43 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPES_H
# define PROTOTYPES_H
/*prints.c*/
void		print_intro(void);
void		print_outro(void);
void		print_bvh_stats(t_bvh *bvh);
void		print_bvh_build_time(struct timeval t_start, struct timeval t_end);
void		print_render_time_for_image(struct timeval t_start, struct timeval t_end);
void		print_tri_count(int counter);
/*checker.c*/
void		check_input(int ac, char **av, t_data *data);
/*identifiers.c*/
int			is_object_file(char *specs);
int			is_light(char *specs);
int			is_cam(char *specs);
int			is_ambient(char *specs);
int			is_plane(char *specs);
int			is_rectangle(char *specs);
/*identifiers_2.c*/
int			is_sphere(char *specs);
int			is_cylinder(char *specs);
void		increase_if_uniq(int *value, char *specs, t_data *data);
int			is_face(char *specs);
int			is_smoothing(char *specs);
/*init.c*/
void		init_rt_lists(t_data *data, t_obj_parser *parsers);
int			init_obj_lists(t_obj_parser *parser);
/*counter.c*/
void		read_rt(t_data *data);
void		read_obj(t_data *data, t_obj_parser *parser);
void		count_rt_scene_elems(char *specs, t_data *data, int *n_cam,
				int *n_ambient);
/*obj_file*/
void		parse_obj_files(t_data *data, char *filename);
int			read_obj_file(t_data *data, t_obj_parser *parser);
int			process_obj_line(char *specs, t_data *data, t_obj_parser *parser);
/*rt_file*/
void		create_elements_rt(t_data *data, char *filename);
/*parser.c*/
void		handle_parsing(char **av, t_data *data);
char		*format_string(char *str, int len);
/*factories.c*/
int			create_ambient_light(t_data *data, char *specs);
int			create_cam(t_data *data, char *specs);
int			create_light(t_data *data, char *specs);
int			create_sphere(t_data *data, char *specs);
int			create_plane(t_data *data, char *specs);
int			create_cylinder(t_data *data, char *specs);
int			create_triangle(t_data *data, char *line, t_obj_parser *parser);
int			create_rectangle(t_data *data, char *specs);
int			set_triangle_properties(t_obj_parser *parser, char *specs);
/*factories_utils.c*/
int			get_ratio(char **specs, float *ratio);
int			get_refraction_index(char **specs, float *ratio);
int			get_rgb_norm(char **specs, float *color);
int			get_rgb(char **specs, unsigned char *color);
int			get_obj_rgb(char **specs, int *color);
int			get_coord(char **specs, float *value);
int			get_normalized_vec(char **specs, float *value);
int			get_fov_range(char **specs, int *fov);
int			get_radius(char **specs, float *radius);
int			get_length(char **specs, float *length);
int			get_pos_float(char **specs, float *size);
/*error.c*/
void		print_error(int error_code);
void		handle_memory_failure(const char *function);
void		handle_file_error(const char *function, const char *filename);
/*free.c*/
void		free_bvh_nonleaf_and_geo(t_bvh *bvh);
void		free_bvh_leaf_and_struct(t_bvh *bvh);
void		free_data_resources(t_data *data);
void		wait_for_render_threads(t_data *data);
void		exit_with_obj_parser_cleanup(t_obj_parser *parsers, int n_files);
void		free_obj_parser_resources(t_obj_parser *parsers, int n_files);
void		free_obj_parser_and_exit_failure(t_obj_parser *parsers, int n_files);
void		abort_scene_parsing_on_failure(t_data *data, char *specs);
/*render.c*/
void		render_first_image(t_data *data);
void		render_pixel(t_calc_img_arg *arg, int p, t_shoot *shoot,
				int (*hd_res)[3]);
void		shoot_ray(t_data *data, t_shoot *shoot);
void		first_rotation_matrix(t_data *data);
void		prepare_image_jobs(t_data *data);
/*checkerboard.c*/
int			eval_checkerboard_pattern(t_shoot *shoot);
/*phong.c*/
void		shading(t_shoot *shoot, t_data *data);
/* BVH */
t_bvh		*init_bvh(t_data *data);
void		remap_objects_after_bvh(t_data *data, t_bvh *bvh);
void		cut_in_two(t_bvh *bvh, int idx, int idx_c, int cut_lvl);
void		get_group_size(t_data *data, t_bvh *bvh);
void		create_root_obj_list(t_data *data, t_bvh *bvh);
int			find_min_idx(int x[3]);
void		malloc_groups_and_geo(t_bvh *bvh, t_cut_in_two *cut);
void		get_mid_planes(t_bvh *bvh, int idx, t_cut_in_two *cut);
t_obj_geo	*create_obj_geo_data(t_bvh *bvh);
void		get_bboxes(t_bvh *bvh, int idx, t_cut_in_two *cut);
void		get_bbox_min_max_root(t_bvh *bvh);
/* tests*/
float		visibility_test_bvh_strict(t_bvh *bvh, int idx, t_shoot *shoot);
float		visibility_test_bvh_fast(t_bvh *bvh, int idx, t_shoot *shoot);
float		shadow_test_bvh(t_shoot *shoot, t_bvh *bvh, int idx,
				float dist_light);
float		visi_tests(t_object *objects, t_shoot *shoot, int n_obj);
float		visi_test_leaves(t_object **objects, t_shoot *shoot, int n_obj);
float		shadow_intersect_objects(t_shoot *shoot, t_object *objects,
				float dist_light, int n_obj);
float		shadow_intersect_leaves(t_shoot *shoot, t_object **objects,
				float dist_light, int n_obj);
float		intersect_sphere(t_object *obj, float ray[3], float origin[3]);
float		intersect_cylinder(t_object *obj, float ray[3], float origin[3]);
float		intersect_plane(t_object *obj, float p_ray[3], float origin[3]);
float		intersect_rectangle(t_object *obj, float ray[3], float origin[3]);
float		intersect_triangle(t_object *obj, float ray[3], float origin[3]);
__m256		aabb_test_simd(t_bvh *bvh, int idx, float dir[3], float src[3]);
void		aabb_test_fast(t_bvh *bvh, int idx, t_shoot *shoot, char res[9]);
void		copy_and_terminate(char *res, int *indices, int size);
/*maths*/
float		dot_vec3(float a[3], float b[3]);
void		matrix34_vec3_multiply_inplace(double a[3][4], float b[3]);
void		matrix33_vec3_multiply_inplace(float a[3][3], float b[3]);
void		matrix33_matrix33_multiply_inplace(float a[3][3], float b[3][3]);
void		matrix44_multiply_inplace(double a[4][4], double b[4][4]);
void		cross_vec3(float a[3], float b[3], float res[3]);
void		normalize(float vector[3], float *magnitude);
int			imin(int a, int b);
int			imax(int a, int b);
void		vec_subtract(float p1[3], float p2[3], float result[3]);
void		copy_vec(float src[3], float dst[3]);
float		triple_scalar(float a[3], float b[3], float c[3]);
void		swap_float(float *t1, float *t2);
float		find_bvh_median(t_bvh *bvh, int idx, int axis);
void		get_rotation_matrix(float cam_dir[3], double t_mat[4][4],
				float c[3]);
void		copy_rotation_matrix_from_tmat(t_data *data);
void		initialize_rotation_matrices(t_data *data);
void		rodrigues_matrix_handler(float u[3], float theta, float c[3],
				double r[4][4]);
void		scale_vec(float v[3], float amp);
/* Multithreading */
void		wait_for_workers(t_data *data);
void		launch_pool(t_data *data);
void		*worker(void *arg);
void		render_image_batch(t_calc_img_arg *arg);
/* mlx hooks */
int			handle_input(int keysym, t_data *data);
int			handle_close(t_data *data);
int			init_mlx(t_mlxlib *data);
void		rotate_cam(t_data *data, float theta, float axis[3], int anti_fa);
void		translate_cam(t_data *data, float v[3], float amp, int anti_fa);
int			mouse_press(int button, int x, int y, void *arg);
int			mouse_release(int button, int x, int y, void *arg);
int			mouse_move(int x, int y, void *arg);
#endif
