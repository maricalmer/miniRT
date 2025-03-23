# include "minirt.h"

int mouse_press(int button, int x, int y, void *arg);
int mouse_release(int button, int x, int y, void *arg);
int mouse_move(int x, int y, void *arg);


int mouse_press(int button, int x, int y, void *arg)
{
    t_data *data = arg;

    if (button == 1)
    {
        data->mouse_pressed_l = 1;
        data->mouse_x = x;
        data->mouse_y = y;

    }
    else if (button == 3)
    {
        data->mouse_pressed_r = 1;
        data->mouse_x = x;
        data->mouse_y = y;
    }
    else if (button == 4)
        translate_cam(data, (float[3]){0, 0, 1}, -CAM_D_TRANS * 0.5, 1);
    else if (button == 5)
        translate_cam(data, (float[3]){0, 0, 1}, CAM_D_TRANS * 0.5, 1);
    return (0);
}

int mouse_release(int button, int x, int y, void *arg)
{
    t_data *data = arg;

    (void)x;
    (void)y;
    if (button == 1)
        data->mouse_pressed_l = 0;
    else if (button == 3)
    {
        data->mouse_pressed_r = 0;
    }
	data->anti_fa = ANTIALIASING_FACT;
    calculate_img(data);
    return (0);
}

int mouse_move(int x, int y, void *arg)
{
    int     dx;
    int     dy;
    float   v[3];
    float   amp;

    t_data *data = arg;
    if (data->mouse_pressed_l)
    {
        dx = x - data->mouse_x;
        dy = y - data->mouse_y;
        if ((dx * dx + dy * dy) >= MOVE_THRESHOLD_2)
        {
            data->mouse_x = x;
            data->mouse_y = y;
            v[0] = -dx;
            v[1] = dy; 
            v[2] = 0;
            normalize(v);
            amp = 5;  // need to be scaled !!!! somehow ?
            translate_cam(data, v, amp, 1);
        }
    }
    if (data->mouse_pressed_r)
    {
        dx = x - data->mouse_x;
        dy = y - data->mouse_y;
        if ((dx * dx + dy * dy) >= MOVE_THRESHOLD_2)
        {
            data->mouse_x = x;
            data->mouse_y = y;
            v[0] = -dy;
            v[1] = -dx;
            v[2] = 0;
            normalize(v);
            rotate_cam(data, 2 * M_PI / 180, v, 1);
        }
    }
    return (0);
}
