/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:52:58 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/14 19:15:55 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Log eating and sleep
// Update last meal time with proper mutex protection
// Put down forks
int	attend_to_eat_sleep(t_philo *philo, size_t i)
{
	pick_forks(philo, i);
	time_log(philo, i, 'e');
	ft_sleep_own(philo->time_eat);
	pthread_mutex_lock(&philo->meal_lock);
	philo->phil[i].last_meal_time = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_lock);
	if (has_simulation_ended(philo) == EXIT_FAILURE && philo->count_meals != -1)
	{
		pthread_mutex_lock(&philo->dinner_lock);
		philo->phil[i].dinner_count += 1;
		pthread_mutex_unlock(&philo->dinner_lock);
	}
	time_log(philo, i, 's');
	release_forks(philo, i);
	ft_sleep_own(philo->time_sleep);
	return (EXIT_SUCCESS);
}

// Modify your think function to record the start time
void	think(size_t i, t_philo *philo)
{
	pthread_mutex_lock(&philo->think_lock);
	pthread_mutex_lock(&philo->simulation_end_lock);
	if (philo->simulation_end != IS_OVER)
	{
		time_log(philo, i, 't');
		pthread_mutex_lock(&philo->think_start_lock);
		philo->phil[i].time_think = (philo->time_die - (get_time_in_ms()
					- philo->phil[i].last_meal_time) - philo->time_eat) / 2;
		pthread_mutex_unlock(&philo->think_start_lock);
	}
	pthread_mutex_unlock(&philo->simulation_end_lock);
	pthread_mutex_unlock(&philo->think_lock);
	if (philo->phil[i].time_think < 0)
		philo->phil[i].time_think = 0;
	if (philo->phil[i].time_think == 0)
		philo->phil[i].time_think = 1;
	if (philo->phil[i].time_think > 600)
		philo->phil[i].time_think = 200;
	ft_sleep_own(philo->phil[i].time_think);
}
