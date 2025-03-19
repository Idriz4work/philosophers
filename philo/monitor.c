/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:46:57 by iatilla-          #+#    #+#             */
/*   Updated: 2025/03/19 17:31:19 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// In your main thread or monitor thread
void *monitor_philosophers(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    size_t i;
    
    while (1)
    {
        for (i = 0; i < philo->n_philo; i++)
        {
            // Check if philosopher has died
            if (check_meal_time(&philo->phil[i], i) == EXIT_FAILURE)
            {
                // Signal all threads to stop
                pthread_mutex_lock(&philo->dead_lock);
                philo->simulation_end = 1;
                pthread_mutex_unlock(&philo->dead_lock);
                
                return NULL;
            }
        }
        
        // Small sleep to avoid hogging CPU
        usleep(1000);
    }
    
    return NULL;
}

