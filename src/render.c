#include "minirt.h"

void	render_first_image(t_data data, void *img)
{
	calculate_ray_prim_dir(data); // => toutes les directions des primary rays are saved.
	for i in [HEIGHT * WIDTH] // iun i pour chaque pixel
		hit_pt, obj	 = intersection_tests (data, i) => the first inter point is returned
		//norm = calculate_normal(data, obj, hit_pt);
		img[i] = phong(data, ); // => color = object color in the first prototype.)
}

intersections_test(t_data data, int i)
{
	obj = data.objects;
	dist_min = float_max;

	while (obj->type != END)
	{
		if (obj->type == SPHERE)
			dist = intersection_test_sphere(); // world_space
		else if (obj->type == PLANE)
			dist = intersection_test_plane(); // world_space
		if (dist && dist < dist_min)
			dist_min = dist;
			hit_obj = obj;
		obj++;
	}
	// calculate hit_pt for closest object (or is it already done in intersection test ??)
	return (hit_pt, hit_obj)

}

/*
calculate the primary ray direction for every pixel in our frame;
save the result in data;
*/
calculate_ray_prim_dir(data);
{
	frame_coord = find_frame(function of cam_position and cam_fov);
	p_center = calculate_pixel_center(frame_coord);
	data->prim_ray_dir[i] = norm_vect(cam_orig, p_center);
}