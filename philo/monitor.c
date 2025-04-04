/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:46:57 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/04 18:19:38 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Modified monitor function to properly handle single philosopher case
// Check if time limit is exceeded
void	*monitor_philosophers(void *arg)
{
	t_philo	*philo;
	time_t	elapsed_time;
	size_t	i;

	philo = (t_philo *)arg;
	 usleep(1000);
	while (1)
	{
		i = 0;
		while (i < philo->n_philo)
		{
			check_messenger(philo, i);
			elapsed_time = time_since_last_meal(philo, i);
			if (elapsed_time >= philo->time_die)
			{
				pthread_mutex_lock(&philo->dead_lock);
				time_log(philo, i, 'd');
				starting_termination(philo);
				pthread_mutex_unlock(&philo->dead_lock);
				return (NULL);  // Exit monitoring thread
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}


// Just detach the thread
int	init_katil(t_philo *philo)
{

	usleep(1000); // Small delay to ensure philosophers start first
	pthread_create(philo->katil, NULL, monitor_philosophers, philo);
	// if (pthread_detach(philo.) != 0)
	// 	return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
