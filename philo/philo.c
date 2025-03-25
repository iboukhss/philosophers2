/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:16:41 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 10:51:39 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

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

// Assuming long is 64-bits

static long	get_time_in_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000L + time.tv_usec / 1000L);
}

static void	sleep_in_ms(int time_in_ms)
{
	usleep(time_in_ms * 1000);
}

static bool	simulation_is_running(t_simulation *sim)
{
	bool	running;

	pthread_mutex_lock(&sim->lock);
	running = sim->is_running;
	pthread_mutex_unlock(&sim->lock);
	return (running);
}

static void	print_timestamp(t_philosopher *philo, const char *msg)
{
	long	time_elapsed;

	if (simulation_is_running(philo->sim))
	{
		pthread_mutex_lock(&philo->sim->print_lock);
		time_elapsed = get_time_in_ms() - philo->sim->start_time;
		printf("%8ld %4d %s\n", time_elapsed, philo->id, msg);
		pthread_mutex_unlock(&philo->sim->print_lock);
	}
}

static void	start_thinking(t_philosopher *philo)
{
	if (simulation_is_running(philo->sim))
	{
		print_timestamp(philo, "is thinking");
		sleep_in_ms(0);
	}
}

static bool	philosopher_has_token(t_philosopher *philo)
{
	bool	has_token;

	pthread_mutex_lock(&philo->lock);
	has_token = philo->has_token;
	pthread_mutex_unlock(&philo->lock);
	return (has_token);
}

static void	drop_token(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->lock);
	philo->has_token = false;
	pthread_mutex_unlock(&philo->lock);
}

static void	give_token(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->lock);
	philo->has_token = true;
	pthread_mutex_unlock(&philo->lock);
}

static void	pass_token(t_philosopher *philo)
{
	t_philosopher	*next;
	t_philosopher	*next_next;

	next = &philo->sim->philos[philo->id % philo->sim->num_philos];
	next_next = &philo->sim->philos[(philo->id + 1) % philo->sim->num_philos];
	drop_token(philo);
	while (simulation_is_running(philo->sim))
	{
		if (!philosopher_has_token(next_next))
		{
			give_token(next);
			return ;
		}
		usleep(25);
	}
}

static void	wait_for_token(t_philosopher *philo)
{
	while (simulation_is_running(philo->sim) && !philosopher_has_token(philo))
	{
		usleep(100);
	}
}

static void	pick_up_forks(t_philosopher *philo)
{
	if (philo->left_fork == philo->right_fork)
	{
		return ;
	}
	if (philo->left_fork < philo->right_fork)
	{
		pthread_mutex_lock(philo->left_fork);
		print_timestamp(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_timestamp(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_timestamp(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_timestamp(philo, "has taken a fork");
	}
}

static void	start_eating(t_philosopher *philo)
{
	if (simulation_is_running(philo->sim))
	{
		pthread_mutex_lock(&philo->lock);
		philo->last_meal_time = get_time_in_ms();
		pthread_mutex_unlock(&philo->lock);
		print_timestamp(philo, "is eating");
		sleep_in_ms(philo->sim->time_to_eat);
		pthread_mutex_lock(&philo->lock);
		philo->meals_eaten++;
		if (philo->sim->max_meals > 0)
		{
			philo->done_eating = (philo->meals_eaten >= philo->sim->max_meals);
		}
		pthread_mutex_unlock(&philo->lock);
	}
}

static void	put_down_forks(t_philosopher *philo)
{
	if (philo->left_fork == philo->right_fork)
	{
		return ;
	}
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	start_sleeping(t_philosopher *philo)
{
	if (simulation_is_running(philo->sim))
	{
		print_timestamp(philo, "is sleeping");
		sleep_in_ms(philo->sim->time_to_sleep);
	}
}

static bool	philosopher_is_done_eating(t_philosopher *philo)
{
	bool	done_eating;

	pthread_mutex_lock(&philo->lock);
	done_eating = philo->done_eating;
	pthread_mutex_unlock(&philo->lock);
	return (done_eating);
}

static bool	philosopher_died(t_philosopher *philo)
{
	bool	died;
	long	time_elapsed;

	pthread_mutex_lock(&philo->lock);
	if (philo->last_meal_time == -1 || philo->done_eating)
	{
		died = false;
	}
	else
	{
		time_elapsed = get_time_in_ms() - philo->last_meal_time;
		died = (time_elapsed > philo->sim->time_to_die);
	}
	pthread_mutex_unlock(&philo->lock);
	return (died);
}

static void	stop_simulation(t_simulation *sim)
{
	pthread_mutex_lock(&sim->lock);
	sim->is_running = false;
	pthread_mutex_unlock(&sim->lock);
}

static void	monitor_meals(t_simulation *sim)
{
	for (int i = 0; i < sim->num_philos; i++)
	{
		if (!philosopher_is_done_eating(&sim->philos[i]))
		{
			return ;
		}
	}
	stop_simulation(sim);
}

static void	monitor_deaths(t_simulation *sim)
{
	for (int i = 0; i < sim->num_philos; i++)
	{
		if (philosopher_died(&sim->philos[i]))
		{
			print_timestamp(&sim->philos[i], "died");
			stop_simulation(sim);
			return ;
		}
	}
}

static void	*monitor_routine(void *arg)
{
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	while (simulation_is_running(sim))
	{
		monitor_meals(sim);
		monitor_deaths(sim);
		sleep_in_ms(2);
	}
	return (NULL);
}

static void	*philo_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	pthread_mutex_lock(&philo->lock);
	philo->start_time = get_time_in_ms();
	philo->last_meal_time = philo->start_time;
	pthread_mutex_unlock(&philo->lock);
	while (simulation_is_running(philo->sim) && !philosopher_is_done_eating(philo))
	{
		start_thinking(philo);
		wait_for_token(philo);
		pick_up_forks(philo);
		start_eating(philo);
		put_down_forks(philo);
		pass_token(philo);
		start_sleeping(philo);
	}
	return (NULL);
}

static int	simulation_init(t_simulation *sim, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		return (1);
	}
	sim->num_philos = atoi(argv[1]);
	sim->time_to_die = atoi(argv[2]);
	sim->time_to_eat = atoi(argv[3]);
	sim->time_to_sleep = atoi(argv[4]);
	sim->max_meals = (argc == 6) ? atoi(argv[5]) : -1;
	sim->philos = malloc(sim->num_philos * sizeof(*sim->philos));
	sim->forks = malloc(sim->num_philos * sizeof(*sim->forks));
	for (int i = 0; i < sim->num_philos; i++)
	{
		pthread_mutex_init(&sim->forks[i], NULL);
	}
	for (int i = 0; i < sim->num_philos; i++)
	{
		sim->philos[i].id = i + 1;
		sim->philos[i].left_fork = &sim->forks[i];
		sim->philos[i].right_fork = &sim->forks[(i + 1) % sim->num_philos];
		sim->philos[i].meals_eaten = 0;
		sim->philos[i].start_time = -1;
		sim->philos[i].last_meal_time = -1;
		sim->philos[i].done_eating = false;
		if (sim->num_philos % 2 == 0)
		{
			sim->philos[i].has_token = (i % 2 == 0) ? true : false;
		}
		else
		{
			sim->philos[i].has_token = (i != sim->num_philos - 1 && i % 2 == 0) ? true : false;
		}
		pthread_mutex_init(&sim->philos[i].lock, NULL);
		sim->philos[i].sim = sim;
	}
	sim->start_time = -1;
	sim->stop_time = -1;
	sim->is_running = true;
	pthread_mutex_init(&sim->lock, NULL);
	pthread_mutex_init(&sim->print_lock, NULL);
	return (0);
}

static int	simulation_start(t_simulation *sim)
{
	sim->start_time = get_time_in_ms();
	pthread_create(&sim->monitor, NULL, monitor_routine, sim);
	for (int i = 0; i < sim->num_philos; i++)
	{
		pthread_create(&sim->philos[i].thread, NULL, philo_routine, &sim->philos[i]);
	}
	return (0);
}

static int	simulation_wait(t_simulation *sim)
{
	pthread_join(sim->monitor, NULL);
	for (int i = 0; i < sim->num_philos; i++)
	{
		pthread_join(sim->philos[i].thread, NULL);
	}
	sim->stop_time = get_time_in_ms();
	return (0);
}

static int	simulation_destroy(t_simulation *sim)
{
	for (int i = 0; i < sim->num_philos; i++)
	{
		pthread_mutex_destroy(&sim->forks[i]);
		pthread_mutex_destroy(&sim->philos[i].lock);
	}
	free(sim->forks);
	free(sim->philos);
	pthread_mutex_destroy(&sim->lock);
	pthread_mutex_destroy(&sim->print_lock);
	return (0);
}

int	main(int argc, char **argv)
{
	t_simulation	sim;

	if (simulation_init(&sim, argc, argv) != 0)
	{
		return (1);
	}
	simulation_start(&sim);
	simulation_wait(&sim);
	simulation_destroy(&sim);
	return (0);
}
