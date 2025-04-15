/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:42:51 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/04/15 18:53:40 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	init_data(t_data *data);


int	main(int ac, char **av)
{
	t_data	data;

	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
	_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
	print_intro();
	init_data(&data);
	check_input(ac, av, &data);
	handle_parsing(av, &data);
	data.cam.mode = 0;
	launch_pool(&data);
	if (init_mlx(&data.mlx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	render_first_image(&data);
	mlx_key_hook(data.mlx.win, &handle_input, &data);
	mlx_hook(data.mlx.win, 4, 1L << 2, &mouse_press, &data);
	mlx_hook(data.mlx.win, 5, 1L << 3, &mouse_release, &data);
	mlx_hook(data.mlx.win, 6, 1L << 6, &mouse_move, &data);
	mlx_hook(data.mlx.win, 17, 0, &handle_close, &data);
	mlx_loop(data.mlx.mlx);
	return (1);
}

static void	init_data(t_data *data)
{
	data->n_obj = 0;
	data->n_light = 0;
	data->n_obj_files = 0;
	data->objects_idx = 0;
	data->obj_fd = -1;
	data->rt_fd = -1;
	data->exit = 0;
	data->anti_fa = ANTIALIASING_FACT;
	data->mouse_pressed_l = 0;
	data->mouse_pressed_r = 0;
	data->cam.mode = 0;
	srand(123456);
}
