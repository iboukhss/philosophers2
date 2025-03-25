/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:16:48 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/25 11:17:16 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	simulation_is_running(t_simulation *sim)
{
	bool	running;

	pthread_mutex_lock(&sim->lock);
	running = sim->is_running;
	pthread_mutex_unlock(&sim->lock);
	return (running);
}

void	stop_simulation(t_simulation *sim)
{
	pthread_mutex_lock(&sim->lock);
	sim->is_running = false;
	pthread_mutex_unlock(&sim->lock);
}
