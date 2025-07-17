#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->philo_count == 1)
	{
		safe_print(philo->data, philo->id, "has taken a fork");
		precise_sleep(philo->data->time_to_die);
		return (NULL);
	}
	while (1)
	{
		pthread_mutex_lock(&philo->data->data_mutex);
		if (philo->data->simulation_ended)
		{
			pthread_mutex_unlock(&philo->data->data_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->data->data_mutex);
		if (!take_forks(philo))
			break;
		eat_routine(philo);
		sleep_routine(philo);
		think_routine(philo);
	}
	return (NULL);
}

int	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		safe_print(philo->data, philo->id, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->mutex);
		safe_print(philo->data, philo->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork->mutex);
		safe_print(philo->data, philo->id, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork->mutex);
		safe_print(philo->data, philo->id, "has taken a fork");
	}
	return (1);
}

void	eat_routine(t_philo *philo)
{
	safe_print(philo->data, philo->id, "is eating");
	pthread_mutex_lock(&philo->data->data_mutex);
	philo->last_meal_time = get_current_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->data_mutex);
	precise_sleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_unlock(&philo->right_fork->mutex);
}

void	sleep_routine(t_philo *philo)
{
	safe_print(philo->data, philo->id, "is sleeping");
	precise_sleep(philo->data->time_to_sleep);
}

void	think_routine(t_philo *philo)
{
	safe_print(philo->data, philo->id, "is thinking");
} 