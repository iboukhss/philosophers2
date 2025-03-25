/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:07:21 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 12:07:52 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pick_up_forks(t_philosopher *philo)
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

void	put_down_forks(t_philosopher *philo)
{
	if (philo->left_fork == philo->right_fork)
	{
		return ;
	}
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
