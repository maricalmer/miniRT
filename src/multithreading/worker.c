/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:00:48 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 16:24:00 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Manages a pool of worker threads to process image calculations in          */
/* parallel. Threads are launched to process rows of the image by accessing   */
/* a shared job list. Mutexes ensure thread safety while updating the         */
/* the joblist. The parent thread waits for all workers to complete their     */
/* tasks before exiting.                                                      */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	*worker(void *arg)
{
	t_data	*data;
	int		joblist_index;

	data = arg;
	while (!data->exit)
	{
		if (data->next_job_id < HEIGHT)
		{
			pthread_mutex_lock(&data->joblist_mutex);
			if (data->next_job_id == HEIGHT)
			{
				pthread_mutex_unlock(&data->joblist_mutex);
				continue ;
			}
			data->active_threads++;
			joblist_index = data->next_job_id;
			data->next_job_id++;
			pthread_mutex_unlock(&data->joblist_mutex);
			calculate_img_packet(&data->joblist[joblist_index]);
			data->active_threads--;
		}
		usleep(USLEEP_WORKER);
	}
	return (NULL);
}

void	wait_for_workers(t_data *data)
{
	while (data->next_job_id < HEIGHT || data->active_threads > 0)
		usleep(USLEEP_PARENT);
}

void	launch_pool(t_data *data)
{
	int	i;

	pthread_mutex_init(&data->joblist_mutex, NULL);
	data->active_threads = 0;
	data->next_job_id = HEIGHT;
	i = -1;
	while (++i < N_THREAD)
		pthread_create(&data->threads[i], NULL, worker, data);
}
