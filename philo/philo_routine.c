/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:14:04 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 11:25:39 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <unistd.h>

static void	start_thinking(t_philosopher *philo)
{
	if (simulation_is_running(philo->sim))
	{
		print_timestamp(philo, "is thinking");
		sleep_in_ms(0);
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

static void	start_sleeping(t_philosopher *philo)
{
	if (simulation_is_running(philo->sim))
	{
		print_timestamp(philo, "is sleeping");
		sleep_in_ms(philo->sim->time_to_sleep);
	}
}

void	*philo_routine(void *arg)
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
