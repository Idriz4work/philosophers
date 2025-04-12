/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:52:58 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/12 18:54:34 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// no mutex lock for eat becasue multiple need to eat,
// mutex lock for forks / chopsticks
// unlocks the mutexes so that the next philosophers can continue in the next round with eating
// Modify attend_to_eat or wherever you take the first fork to calculate thinking time
int	attend_to_eat(t_philo *philo, size_t i)
{
	if (philo->simulation_end != IS_OVER)
	{
		pthread_mutex_lock(&philo->chop_sticks[philo->phil[i].left_chop]);
		time_log(philo, i, 'f');
		pthread_mutex_lock(&philo->chop_sticks[philo->phil[i].right_chop]);
		time_log(philo, i, 'f');
		philo->phil[i].last_meal_time = get_time_in_ms();
		time_log(philo, i, 'e');
		ft_sleep_own(philo->time_eat);
		pthread_mutex_unlock(&philo->chop_sticks[philo->phil[i].left_chop]);
		pthread_mutex_unlock(&philo->chop_sticks[philo->phil[i].right_chop]);
	}
	return (EXIT_SUCCESS);
}

// function that sends philo to sleep
void	sleep_routine(t_philo *philo, size_t i)
{
	if (philo->simulation_end != IS_OVER)
	{
		time_log(philo, i, 's');
		ft_sleep_own(philo->time_sleep);
	}
}

// Calculate and update thinking time
void	update_thinking_time(t_attr *philo)
{
	long long	current_time;
	long		thinking_duration;

	if (philo->thinking_start_time > 0)
	{
		current_time = get_time_in_ms();
		thinking_duration = current_time - philo->thinking_start_time;
		philo->total_thinking_time += thinking_duration;
		philo->thinking_start_time = 0;
	}
}

// Modify your think function to record the start time
void	think(size_t i, t_philo *philo)
{
	if (philo->simulation_end != IS_OVER)
	{
		time_log(philo, i, 't');
		// philo->phil[i].state = IS_THINKING;
		philo->phil[i].thinking_start_time = get_time_in_ms();
	}
}
