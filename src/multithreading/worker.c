#include "minirt.h"

int	check_mutex_var(int var, pthread_mutex_t mutex);

void	*worker(void *arg)
{
	t_data	*data;
	int		joblist_top;

	data = arg;
	while (!data->exit)
	{
		if (atomic_load(&data->joblist_top) < HEIGHT)
		{
			pthread_mutex_lock(&data->joblist_mutex);
			if (atomic_load(&data->joblist_top) == HEIGHT)
			{
				pthread_mutex_unlock(&data->joblist_mutex);
				continue ;
			}
			data->active_threads++;
			joblist_top = data->joblist_top;
			atomic_fetch_add(&data->joblist_top, 1);
			pthread_mutex_unlock(&data->joblist_mutex);
			calculate_img_packet(&data->joblist[joblist_top]);
			atomic_fetch_add(&data->active_threads, -1);
		}
		usleep(USLEEP_WORKER);
	}
	return(NULL);
}

/* check if it works like that or if we need atomic or mutex protection to have the L1 cache of the parent uptodate. */
void wait_for_workers(t_data *data)
{
	//while (atomic_load(&data->joblist_size) > 0 || atomic_load(&data->active_threads) > 0)
	while (data->joblist_top < HEIGHT || data->active_threads > 0)
		usleep(USLEEP_PARENT);
}

void launch_pool(t_data *data) // initialisation_MT
{
	int i;

	pthread_mutex_init(&data->joblist_mutex, NULL);
	data->active_threads = 0;
	data->joblist_top = HEIGHT;
	i = -1;
	while (++i < N_THREAD)
	{
		pthread_create(&data->threads[i], NULL, worker, data);
	}
}

// int	check_mutex_var(int var, pthread_mutex_t mutex)
// {
// 	int	x;

// 	pthread_mutex_lock(&mutex);
// 	x = var;
// 	pthread_mutex_unlock(&mutex);
// 	return (x);
// }