/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:16:41 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 13:10:05 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <stdlib.h>

static int	init_user_input_parameters(t_simulation *sim, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		return (1);
	}
	sim->num_philos = atoi(argv[1]);
	sim->time_to_die = atoi(argv[2]);
	sim->time_to_eat = atoi(argv[3]);
	sim->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
	{
		sim->max_meals = atoi(argv[5]);
	}
	else
	{
		sim->max_meals = -1;
	}
	return (0);
}

static int	init_philosophers(t_simulation *sim)
{
	for (int i = 0; i < sim->num_philos; i++)
	{
		sim->philos[i].id = i + 1;
		sim->philos[i].left_fork = &sim->forks[i];
		sim->philos[i].right_fork = &sim->forks[(i + 1) % sim->num_philos];
		sim->philos[i].meals_eaten = 0;
		sim->philos[i].start_time = -1;
		sim->philos[i].last_meal_time = -1;
		sim->philos[i].done_eating = false;
		pthread_mutex_init(&sim->philos[i].lock, NULL);
		sim->philos[i].sim = sim;
	}
	return (0);
}

static int	init_forks(pthread_mutex_t *forks, int num_forks)
{
	for (int i = 0; i < num_forks; i++)
	{
		pthread_mutex_init(&forks[i], NULL);
	}
	return (0);
}

static int	distribute_tokens(t_philosopher *philos, int num_philos)
{
	for (int i = 0; i < num_philos; i++)
	{
		if (num_philos % 2 == 0)
		{
			if (i % 2 == 0)
				philos[i].has_token = true;
			else
				philos[i].has_token = false;
		}
		else
		{
			if (i != num_philos - 1 && i % 2 == 0)
				philos[i].has_token = true;
			else
				philos[i].has_token = false;
		}
	}
	return (0);
}

static int	init_simulation(t_simulation *sim)
{
	sim->philos = malloc(sim->num_philos * sizeof(*sim->philos));
	sim->forks = malloc(sim->num_philos * sizeof(*sim->forks));
	init_philosophers(sim);
	init_forks(sim->forks, sim->num_philos);
	distribute_tokens(sim->philos, sim->num_philos);
	sim->start_time = -1;
	sim->stop_time = -1;
	sim->is_running = true;
	pthread_mutex_init(&sim->lock, NULL);
	pthread_mutex_init(&sim->print_lock, NULL);
	return (0);
}

static int	run_simulation(t_simulation *sim)
{
	t_philosopher	*philos;
	int				num_philos;

	philos = sim->philos;
	num_philos = sim->num_philos;
	sim->start_time = get_time_in_ms();
	pthread_create(&sim->monitor, NULL, monitor_routine, sim);
	for (int i = 0; i < num_philos; i++)
	{
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
	}
	pthread_join(sim->monitor, NULL);
	for (int i = 0; i < num_philos; i++)
	{
		pthread_join(sim->philos[i].thread, NULL);
	}
	sim->stop_time = get_time_in_ms();
	return (0);
}

static int	destroy_simulation(t_simulation *sim)
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

	if (init_user_input_parameters(&sim, argc, argv) != 0)
	{
		return (1);
	}
	init_simulation(&sim);
	run_simulation(&sim);
	destroy_simulation(&sim);
	return (0);
}
