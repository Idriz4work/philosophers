/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_active.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:23:52 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/04 12:51:09 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// This function checks if a philosopher will die in 10 milliseconds
// and therefore prints a warning message
void	check_messenger(t_philo *philo, size_t i)
{
	long	current_starve_time;

	pthread_mutex_lock(&philo->print_lock);
	current_starve_time = time_since_last_meal(philo, i);
	if (philo->time_die - current_starve_time <= 10
		&& philo->time_die - current_starve_time > 5
		&& !philo->messenger_state)
	{
		time_log(philo, i, 'M');
		philo->messenger_state = 1;
	}
	pthread_mutex_unlock(&philo->print_lock);
}

// Checks if the time to die is near before 10 seconds of starvation
// => therefore prints message that philo is going to die in 10 ms
// Checks if the time of thinking is longer than the time to die
int	time_check(t_philo *philo, size_t i)
{
	int		result;
	t_attr	*philosopher;
	long	current_starve_time;

	result = EXIT_SUCCESS;
	philosopher = &philo->phil[i];
	current_starve_time = time_since_last_meal(philo, i);
	if (current_starve_time >= philosopher->time_die)
	{
		pthread_mutex_lock(&philo->dead_lock);
		philo->phil[i].state = IS_DEAD;
		result = EXIT_FAILURE;
		pthread_mutex_unlock(&philo->dead_lock);
	}
	return (result);
}

// checks if the number of eating cycles was reached
int	check_dinner(t_philo *philosopher)
{
	if (philosopher->dinner_count == philosopher->dinner_number)
		return (EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
