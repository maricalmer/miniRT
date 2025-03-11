# include "minirt.h"

int mouse_press(int button, int x, int y, void *arg);
int mouse_release(int button, int x, int y, void *arg);
int mouse_move(int x, int y, void *arg);


int mouse_press(int button, int x, int y, void *arg)
{
    t_data *data = arg;
    printf("mouse press\n");
    if (button == 1)
    {

        data->mouse_pressed_l = 1;
        data->mouse_x = x;
        data->mouse_y = y;

    }
    else if (button == 2) //???
    {
        data->mouse_pressed_r = 1;
        data->mouse_x = x;
        data->mouse_y = y;
    }
    return (0);
}

int mouse_release(int button, int x, int y, void *arg)
{
    t_data *data = arg;

    if (button == 1)
    {
        data->mouse_pressed_l = 0;
        printf("left released\n");
    }
    else if (button == 2)
        data->mouse_pressed_r = 0;
    return (0);
}

int mouse_move(int x, int y, void *arg)
{
    int     dx;
    int     dy;
    float   t[3];
    float   amp;

    t_data *data = arg;
    if (data->mouse_pressed_r)
    {
        dx = x - data->mouse_x;
        dy = y - data->mouse_y;
        if ((dx * dx + dy * dy) >= MOVE_THRESHOLD_2)
        {
            printf("translation !!!\n");
            data->mouse_x = x;
            data->mouse_y = y;
            t[0] = dx;
            t[1] = dy;
            t[2] = 0;
            normalize(t);
            amp = 10;  // need to be scaled !!!! somehow ?
            translate_cam(data, t, amp);
        }
    }
    // if (data->mouse_pressed_l)
    // {
    //     dx = x - data->mouse_x;
    //     dy = y - data->mouse_y;
    //     if ((dx * dx + dy * dy) >= MOVE_THRESHOLD_2)
    //     {
    //         data->mouse_x = x;
    //         data->mouse_y = y;
    //         t[0] = dx;
    //         t[1] = dy;
    //         t[2] = 0;
    //         normalize(t);
    //         amp = 10;  // need to be scaled !!!! somehow ?
    //         rotate_cam(data, t, amp);
    //     }
    // }
    return (0);
}