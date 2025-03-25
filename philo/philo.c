/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:16:41 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 13:24:43 by iboukhss         ###   ########.fr       */
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
