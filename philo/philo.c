/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:16:41 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 11:22:01 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <stdlib.h>

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
