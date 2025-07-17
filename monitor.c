/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csamaha <csamaha@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 14:06:28 by csamaha           #+#    #+#             */
/*   Updated: 2025/07/17 14:06:28 by csamaha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_simulation_ended(t_data *data)
{
	int	ended;

	pthread_mutex_lock(&data->data_mutex);
	ended = data->simulation_ended;
	pthread_mutex_unlock(&data->data_mutex);
	return (ended);
}

static int	check_philosophers_death(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (check_philosopher_death(&data->philosophers[i]))
		{
			announce_death(&data->philosophers[i]);
			end_simulation(data);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_meal_completion(t_data *data)
{
	if (data->required_meals != -1 && check_all_philosophers_fed(data))
	{
		end_simulation(data);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_simulation_ended(data))
			break ;
		if (check_philosophers_death(data))
			return (NULL);
		if (check_meal_completion(data))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}

int	check_philosopher_death(t_philo *philo)
{
	long	current_time;
	long	time_since_meal;
	int		dead;

	pthread_mutex_lock(&philo->data->data_mutex);
	if (philo->data->simulation_ended)
	{
		pthread_mutex_unlock(&philo->data->data_mutex);
		return (0);
	}
	current_time = get_current_time_ms();
	time_since_meal = current_time - philo->last_meal_time;
	dead = (time_since_meal >= philo->data->time_to_die);
	pthread_mutex_unlock(&philo->data->data_mutex);
	return (dead);
}
