/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:13:22 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 16:13:35 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

// Assuming long is 64-bits

long	get_time_in_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000L + time.tv_usec / 1000L);
}

void	sleep_in_ms(int time_in_ms)
{
	usleep(time_in_ms * 1000);
}

void	print_timestamp(t_philosopher *philo, const char *msg)
{
	long	time_elapsed;

	if (simulation_is_running(philo->sim))
	{
		pthread_mutex_lock(&philo->sim->print_lock);
		time_elapsed = get_time_in_ms() - philo->sim->start_time;
		printf("%8ld %4d %s\n", time_elapsed, philo->id, msg);
		pthread_mutex_unlock(&philo->sim->print_lock);
	}
}

int	my_atoi(const char *str)
{
	int	res;

	res = 0;
	while (*str >= '0' && *str <= '9')
	{
		res *= 10;
		res += *str - '0';
		str++;
	}
	return (res);
}
