/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:01:34 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 12:03:35 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <unistd.h>

void	wait_for_token(t_philosopher *philo)
{
	while (simulation_is_running(philo->sim) && !philosopher_has_token(philo))
	{
		usleep(100);
	}
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

void	pass_token(t_philosopher *philo)
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
