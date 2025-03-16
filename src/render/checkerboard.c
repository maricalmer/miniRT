#include "minirt.h"

// return 1 on color1 and 0 on color2
int	check_checkerboard_grid(t_shoot *shoot)
{
	float	u;
	float	v;
	int		u_idx;
	int		v_idx;
	float	oc[3];

	vec_substr(shoot->hit_pt, shoot->obj->geo.pl.point, oc);
	u = dot_13_13(oc, shoot->obj->geo.pl.u);
	v = dot_13_13(oc, shoot->obj->geo.pl.v);
	if (u < 0)
		u_idx = u / CHECKER_SIZE - 1;
	else
		u_idx = u / CHECKER_SIZE;
	if (v < 0)
		v_idx = v / CHECKER_SIZE - 1;
	else
		v_idx = v / CHECKER_SIZE;
	if ((u_idx + v_idx) % 2)
		return (1);
	return (0);
}
