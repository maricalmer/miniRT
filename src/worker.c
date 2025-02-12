#include "minirt.h"

void	*worker(void *arg)
{
	t_data	*data;
	t_job	*job;

	data = arg;
	while (1)
	{
		if (data->joblist_size)
		{
			pthread_mutex_lock(&data->joblist_mutex);
			data->active_threads++;
			job = data->joblist + data->joblist_top;
			data->joblist_top++;
			data->joblist_size--;
			pthread_mutex_unlock(&data->joblist_mutex);
			job->function(job->arg);
			data->active_threads--;
		}
		usleep(USLEEP_WORKER);
	}
}

void launch_pool(t_data *data) // initialisation_MT
{
	int i;

	pthread_mutex_init(&data->joblist_mutex, NULL);
	data->active_threads = 0;
	data->joblist_size = 0;
	i = -1;
	while (++i < N_THREAD)
	{
		pthread_create(&data->threads[i], NULL, worker, data);
	}
}

/* check if it works like that or if we need atomic or mutex protection to have the L1 cache of the parent uptodate. */
void wait_for_workers(t_data *data)
{
	while (data->joblist_size > 0 || data->active_threads > 0)
		usleep(USLEEP_PARENT);
}
