/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csamaha <csamaha@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 14:06:31 by csamaha           #+#    #+#             */
/*   Updated: 2025/07/17 14:06:31 by csamaha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				id;
}	t_fork;

typedef struct s_philo
{
	int				id;
	long			last_meal_time;
	int				meals_eaten;
	t_fork			*left_fork;
	t_fork			*right_fork;
	struct s_data	*data;
	pthread_t		thread;
}	t_philo;

typedef struct s_data
{
	int				philo_count;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				required_meals;
	long			start_time;
	int				simulation_ended;
	t_philo			*philosophers;
	t_fork			*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	data_mutex;
	pthread_t		monitor_thread;
}	t_data;

/* main.c */
int		validate_arguments(int argc, char **argv);
int		parse_arguments(int argc, char **argv, t_data *data);
int		start_simulation(t_data *data);
void	cleanup_and_exit(t_data *data, int exit_code);

/* init.c */
int		init_data_structure(t_data *data);
int		allocate_memory(t_data *data);
int		init_mutexes(t_data *data);
int		init_philosophers(t_data *data);
int		init_forks(t_data *data);

/* philosopher.c */
void	*philosopher_routine(void *arg);
int		take_forks(t_philo *philo);
void	eat_routine(t_philo *philo);
void	sleep_routine(t_philo *philo);
void	think_routine(t_philo *philo);

/* monitor.c */
void	*monitor_routine(void *arg);
int		check_philosopher_death(t_philo *philo);
int		check_all_philosophers_fed(t_data *data);
void	announce_death(t_philo *philo);
void	end_simulation(t_data *data);

/* utils.c */
long	get_current_time_ms(void);
void	safe_print(t_data *data, int philo_id, char *message);
void	precise_sleep(long duration_ms);
void	cleanup_mutexes(t_data *data);
void	free_allocated_memory(t_data *data);

#endif 