/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:22:55 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 13:22:57 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <stdlib.h>

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

int	init_simulation(t_simulation *sim)
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

int	destroy_simulation(t_simulation *sim)
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
