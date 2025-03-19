#include "minirt.h"

static t_obj_geo extract_geo_data_tri(t_object *obj);
static t_obj_geo extract_geo_data_sphere(t_object *obj);

t_obj_geo *create_obj_geo_data(t_bvh *bvh)
{
	t_object 	*obj;
	t_obj_geo 	*geo_data;
	int		i;

	geo_data = malloc(bvh->group_size[0] * sizeof(t_obj_geo));
	bvh->obj_geo[0] = malloc(sizeof(t_obj_geo *) * bvh->group_size[0]);
	i = -1;
	while (++i < bvh->group_size[0])
	{
		obj = bvh->group[0][i];
		if (obj->type == SPHERE)
			geo_data[i] = extract_geo_data_sphere(obj);
		if (obj->type == TRI)
			geo_data[i] = extract_geo_data_tri(obj);
		bvh->obj_geo[0][i] = &geo_data[i];	
	}
	return (geo_data);
}

static t_obj_geo extract_geo_data_sphere(t_object *obj)
{
	t_obj_geo 	geo;
	int			i;

	i = -1;
	while (++i < 3)
	{
		geo.center[i] = obj->geo.sph.center[i];
		geo.bmin[i] = obj->geo.sph.center[i] - obj->geo.sph.radius;
		geo.bmax[i] = obj->geo.sph.center[i] + obj->geo.sph.radius;
	}
	return (geo);
}

static t_obj_geo extract_geo_data_tri(t_object *obj)
{
	t_obj_geo 	geo;
	int			i;

	i = -1;
	while (++i < 3)
	{
		geo.center[i] = (obj->geo.tri.v0[i] + obj->geo.tri.v1[i] + obj->geo.tri.v2[i]) / 3; // or the mid of min/max instead ??
		geo.bmin[i] = fmin(obj->geo.tri.v0[i] , fmin(obj->geo.tri.v1[i], obj->geo.tri.v2[i]));
		geo.bmax[i] = fmax(obj->geo.tri.v0[i] , fmax(obj->geo.tri.v1[i], obj->geo.tri.v2[i]));
	}
	return (geo);
}
