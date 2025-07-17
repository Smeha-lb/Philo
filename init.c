#include "philo.h"

int	init_data_structure(t_data *data)
{
	data->simulation_ended = 0;
	if (!allocate_memory(data))
		return (0);
	if (!init_mutexes(data))
		return (0);
	if (!init_forks(data))
		return (0);
	if (!init_philosophers(data))
		return (0);
	return (1);
}

int	allocate_memory(t_data *data)
{
	data->philosophers = malloc(sizeof(t_philo) * data->philo_count);
	if (!data->philosophers)
		return (0);
	data->forks = malloc(sizeof(t_fork) * data->philo_count);
	if (!data->forks)
	{
		free(data->philosophers);
		return (0);
	}
	memset(data->philosophers, 0, sizeof(t_philo) * data->philo_count);
	memset(data->forks, 0, sizeof(t_fork) * data->philo_count);
	return (1);
}

int	init_mutexes(t_data *data)
{
	int	i;

	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->data_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		return (0);
	}
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&data->forks[i].mutex, NULL) != 0)
		{
			while (i > 0)
				pthread_mutex_destroy(&data->forks[--i].mutex);
			pthread_mutex_destroy(&data->print_mutex);
			pthread_mutex_destroy(&data->data_mutex);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].data = data;
		data->philosophers[i].left_fork = &data->forks[i];
		if (i == data->philo_count - 1)
			data->philosophers[i].right_fork = &data->forks[0];
		else
			data->philosophers[i].right_fork = &data->forks[i + 1];
		i++;
	}
	return (1);
}

int	init_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		data->forks[i].id = i + 1;
		i++;
	}
	return (1);
} 