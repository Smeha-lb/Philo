/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csamaha <csamaha@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 14:15:33 by csamaha           #+#    #+#             */
/*   Updated: 2025/07/17 14:15:33 by csamaha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_all_philosophers_fed(t_data *data)
{
	int	i;
	int	fed_count;

	if (data->required_meals == -1)
		return (0);
	fed_count = 0;
	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&data->data_mutex);
		if (data->philosophers[i].meals_eaten >= data->required_meals)
			fed_count++;
		pthread_mutex_unlock(&data->data_mutex);
		i++;
	}
	if (fed_count == data->philo_count)
		return (1);
	return (0);
}

void	announce_death(t_philo *philo)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->data_mutex);
	if (!philo->data->simulation_ended)
	{
		philo->data->simulation_ended = 1;
		timestamp = get_current_time_ms() - philo->data->start_time;
		pthread_mutex_unlock(&philo->data->data_mutex);
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%ld %d died\n", timestamp, philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
	else
	{
		pthread_mutex_unlock(&philo->data->data_mutex);
	}
}

void	end_simulation(t_data *data)
{
	pthread_mutex_lock(&data->data_mutex);
	data->simulation_ended = 1;
	pthread_mutex_unlock(&data->data_mutex);
}
