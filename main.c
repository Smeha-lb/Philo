/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csamaha <csamaha@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 14:06:04 by csamaha           #+#    #+#             */
/*   Updated: 2025/07/17 14:06:04 by csamaha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (!validate_arguments(argc, argv))
	{
		printf("Error: Invalid arguments\n");
		printf("Usage: ./philo number_of_philosophers time_to_die ");
		printf("time_to_eat time_to_sleep [number_of_meals]\n");
		return (1);
	}
	if (!parse_arguments(argc, argv, &data))
	{
		printf("Error: Failed to parse arguments\n");
		return (1);
	}
	if (!init_data_structure(&data) || !start_simulation(&data))
		cleanup_and_exit(&data, 1);
	cleanup_and_exit(&data, 0);
	return (0);
}

int	validate_arguments(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc < 5 || argc > 6)
		return (0);
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		if (atoi(argv[i]) <= 0)
			return (0);
		i++;
	}
	return (1);
}

int	parse_arguments(int argc, char **argv, t_data *data)
{
	data->philo_count = atoi(argv[1]);
	data->time_to_die = atoi(argv[2]);
	data->time_to_eat = atoi(argv[3]);
	data->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		data->required_meals = atoi(argv[5]);
	else
		data->required_meals = -1;
	if (data->philo_count > 200)
		return (0);
	return (1);
}

int	start_simulation(t_data *data)
{
	int	i;

	data->start_time = get_current_time_ms();
	i = 0;
	while (i < data->philo_count)
	{
		data->philosophers[i].last_meal_time = data->start_time;
		if (pthread_create(&data->philosophers[i].thread, NULL,
				philosopher_routine, &data->philosophers[i]) != 0)
			return (0);
		i++;
	}
	if (pthread_create(&data->monitor_thread, NULL, monitor_routine, data) != 0)
		return (0);
	i = 0;
	while (i < data->philo_count)
	{
		pthread_join(data->philosophers[i].thread, NULL);
		i++;
	}
	pthread_join(data->monitor_thread, NULL);
	return (1);
}

void	cleanup_and_exit(t_data *data, int exit_code)
{
	cleanup_mutexes(data);
	free_allocated_memory(data);
	exit(exit_code);
}
