/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:52:58 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/04 12:27:56 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// no mutex accept for print mutex which is neceassary for stdout
void	eat(size_t i, t_philo *philo)
{
	philo->phil[i].last_meal_time = get_time_in_ms();
	time_log(philo, i, 'e');
	sleep_own(i, philo, 'e');
}

// no mutex lock for eat becasue multiple need to eat,
// mutex lock for forks / chopsticks
// unlocks the mutexes so that the next philosophers can continue in the next round with eating
int	attend_to_eat(t_philo *philo, size_t i)
{
	pthread_mutex_lock(&philo->chop_sticks[philo->phil[i].left_chop]);
	pthread_mutex_lock(&philo->chop_sticks[philo->phil[i].right_chop]);
	time_log(philo, i, 'f');
	time_log(philo, i, 'f');
	eat(i, philo);
	pthread_mutex_unlock(&philo->chop_sticks[philo->phil[i].left_chop]);
	pthread_mutex_unlock(&philo->chop_sticks[philo->phil[i].right_chop]);
	return (EXIT_SUCCESS);
}

// Check for starvation - this should ideally be done by a monitor thread
// or before any action that might take time
void	think(size_t i, t_philo *philo)
{
	philo->phil[i].state = IS_THINKING;
	time_log(philo, i, 't');
	sleep_own(i,philo,'t');
}
