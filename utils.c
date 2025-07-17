#include "philo.h"

long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	safe_print(t_data *data, int philo_id, char *message)
{
	long	timestamp;

	pthread_mutex_lock(&data->print_mutex);
	pthread_mutex_lock(&data->data_mutex);
	if (!data->simulation_ended)
	{
		timestamp = get_current_time_ms() - data->start_time;
		printf("%ld %d %s\n", timestamp, philo_id, message);
	}
	pthread_mutex_unlock(&data->data_mutex);
	pthread_mutex_unlock(&data->print_mutex);
}

void	precise_sleep(long duration_ms)
{
	long	start_time;
	long	current_time;

	start_time = get_current_time_ms();
	while (1)
	{
		current_time = get_current_time_ms();
		if (current_time - start_time >= duration_ms)
			break;
		usleep(500);
	}
}

void	cleanup_mutexes(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_destroy(&data->forks[i].mutex);
			i++;
		}
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->data_mutex);
}

void	free_allocated_memory(t_data *data)
{
	if (data->philosophers)
	{
		free(data->philosophers);
		data->philosophers = NULL;
	}
	if (data->forks)
	{
		free(data->forks);
		data->forks = NULL;
	}
} 