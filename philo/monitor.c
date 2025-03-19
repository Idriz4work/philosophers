/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:46:57 by iatilla-          #+#    #+#             */
/*   Updated: 2025/03/19 18:01:03 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Create monitor / katil thread
// Join monitor thread
int	init_katil(t_philo *philo)
{
	pthread_t	moni_thread;

	if (pthread_create(&moni_thread, NULL, monitor_philosophers, philo) == -1)
		return (EXIT_FAILURE);
	if (pthread_join(moni_thread, NULL) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

// In your main thread or monitor thread
// Check if philosopher has died
// Small sleep to avoid hogging CPU
void	*monitor_philosophers(void *arg)
{
	t_philo	*philo;
	size_t	i;

	philo = (t_philo *)arg;
	i = 0;
	while (1)
	{
		while (i < philo->n_philo)
		{
			if (check_meal_time(&philo->phil[i], i) == EXIT_FAILURE)
			{
				pthread_mutex_lock(&philo->dead_lock);
				philo->simulation_end = 1;
				pthread_mutex_unlock(&philo->dead_lock);
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
