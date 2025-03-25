/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:12:08 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 11:12:58 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	*monitor_routine(void *arg)
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
