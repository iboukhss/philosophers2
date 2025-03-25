/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:16:41 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 16:51:35 by iboukhss         ###   ########.fr       */
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
	sim->num_philos = my_atoi(argv[1]);
	sim->time_to_die = my_atoi(argv[2]);
	sim->time_to_eat = my_atoi(argv[3]);
	sim->time_to_sleep = my_atoi(argv[4]);
	if (argc == 6)
	{
		sim->max_meals = my_atoi(argv[5]);
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
	int				i;

	philos = sim->philos;
	num_philos = sim->num_philos;
	sim->start_time = get_time_in_ms();
	pthread_create(&sim->monitor, NULL, monitor_routine, sim);
	i = 0;
	while (i < num_philos)
	{
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
		i++;
	}
	pthread_join(sim->monitor, NULL);
	i = 0;
	while (i < num_philos)
	{
		pthread_join(sim->philos[i].thread, NULL);
		i++;
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
