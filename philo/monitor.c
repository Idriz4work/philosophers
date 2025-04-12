/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:46:57 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/12 18:53:21 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// rules that determine to die
// 1. Late eating
// 2. Sleep too long
int	death_rules(t_philo *philo, size_t i)
{
	time_t	lst_meal_time;

	// Check time since last meal - this is the most important rule
	lst_meal_time = time_since_last_meal(philo, i);
	if (lst_meal_time >= philo->time_die)
		return (EXIT_FAILURE);
	// if (philo->phil[i].s)
	return (EXIT_SUCCESS);
}

// Modified monitor function to properly handle single philosopher case
// Check if time limit is exceeded
void	*monitor_philosophers(void *arg)
{
	t_philo	*philo;
	size_t	i;

	philo = (t_philo *)arg;
	usleep(1000);
	if (philo->n_philo == 1)
		return (NULL);
	while (1)
	{
		i = 0;
		while (i < philo->n_philo)
		{
			check_messenger(philo, i);
			if (death_rules(philo, i) == EXIT_FAILURE)
			{
				pthread_mutex_lock(&philo->dead_lock);
				starting_termination(philo);
				time_log(philo, i, 'd');
				pthread_mutex_unlock(&philo->dead_lock);
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

// In init_katil:
int init_katil(t_philo *philo)
{
    // Small delay to ensure philosophers start first
    usleep(1000);
    if (philo->n_philo >= 1)
    {
        if (pthread_create(philo->katil, NULL, monitor_philosophers, philo) != 0)
        {
            printf("Error creating monitor thread\n");
            return (EXIT_FAILURE);
        }
    }
    return (EXIT_SUCCESS);
}