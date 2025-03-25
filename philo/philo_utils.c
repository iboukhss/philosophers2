/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:15:55 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 11:16:26 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	philosopher_has_token(t_philosopher *philo)
{
	bool	has_token;

	pthread_mutex_lock(&philo->lock);
	has_token = philo->has_token;
	pthread_mutex_unlock(&philo->lock);
	return (has_token);
}

bool	philosopher_is_done_eating(t_philosopher *philo)
{
	bool	done_eating;

	pthread_mutex_lock(&philo->lock);
	done_eating = philo->done_eating;
	pthread_mutex_unlock(&philo->lock);
	return (done_eating);
}

bool	philosopher_died(t_philosopher *philo)
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
