/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:10:32 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 12:17:49 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>

typedef struct s_simulation		t_simulation;
typedef struct s_philosopher	t_philosopher;

typedef struct s_simulation
{
	int				num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				max_meals;
	t_philosopher	*philos;
	pthread_mutex_t	*forks;
	pthread_t		monitor;
	long			start_time;
	long			stop_time;
	bool			is_running;
	pthread_mutex_t	lock;
	pthread_mutex_t	print_lock;
}	t_simulation;

typedef struct s_philosopher
{
	int				id;
	pthread_t		thread;
	long			start_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				meals_eaten;
	long			last_meal_time;
	bool			done_eating;
	bool			has_token;
	pthread_mutex_t	lock;
	t_simulation	*sim;
}	t_philosopher;

// Main utils
long	get_time_in_ms(void);
void	sleep_in_ms(int time_in_ms);
void	print_timestamp(t_philosopher *philo, const char *msg);

// Simulation utils
bool	simulation_is_running(t_simulation *sim);
void	stop_simulation(t_simulation *sim);

// Philosopher utils
bool	philosopher_has_token(t_philosopher *philo);
bool	philosopher_is_done_eating(t_philosopher *philo);
bool	philosopher_died(t_philosopher *philo);

// Token utils
void	wait_for_token(t_philosopher *philo);
void	pass_token(t_philosopher *philo);

// Fork utils
void	pick_up_forks(t_philosopher *philo);
void	put_down_forks(t_philosopher *philo);

// Main routines
void	*philo_routine(void *arg);
void	*monitor_routine(void *arg);

#endif
