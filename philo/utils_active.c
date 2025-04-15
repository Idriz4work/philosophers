/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_active.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:23:52 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/15 00:40:13 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// goes throu all philos & checks if all philosophers had
// the amount of meals they need to have
// j means that 1 philsoopher ate his meals
// if j == number_philos then all philos had have they meals
// & therefore can stop return exitsuccess
int	check_for_dinner(t_philo *philos)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < philos->n_philo)
	{
		pthread_mutex_lock(&philos->dinner_lock);
		if (philos->phil[i].dinner_count == philos->count_meals)
			j++;
		pthread_mutex_unlock(&philos->dinner_lock);
		i++;
	}
	if (j == philos->n_philo)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

//
void	pick_forks(t_philo *philo, size_t i)
{
	if (i % 2 == 0)
	{
		pthread_mutex_lock(&philo->chop_sticks[philo->phil[i].left_chop]);
		time_log(philo, i, 'f');
		pthread_mutex_lock(&philo->chop_sticks[philo->phil[i].right_chop]);
		time_log(philo, i, 'f');
	}
	else
	{
		pthread_mutex_lock(&philo->chop_sticks[philo->phil[i].left_chop]);
		time_log(philo, i, 'f');
		pthread_mutex_lock(&philo->chop_sticks[philo->phil[i].right_chop]);
		time_log(philo, i, 'f');
	}
}

//
void	release_forks(t_philo *philo, size_t i)
{
	if (i % 2 == 0)
	{
		pthread_mutex_unlock(&philo->chop_sticks[philo->phil[i].right_chop]);
		pthread_mutex_unlock(&philo->chop_sticks[philo->phil[i].left_chop]);
	}
	else
	{
		pthread_mutex_unlock(&philo->chop_sticks[philo->phil[i].left_chop]);
		pthread_mutex_unlock(&philo->chop_sticks[philo->phil[i].right_chop]);
	}
}

// This function checks if a philosopher will die in 10 milliseconds
// and therefore prints a warning message
void	check_messenger(t_philo *philo, size_t i)
{
	long	current_starve_time;
	long	time_left;

	current_starve_time = time_since_last_meal(philo, i);
	time_left = philo->time_die - current_starve_time;
	if (time_left <= 10 && time_left > 0 && !philo->messenger_state)
	{
		pthread_mutex_lock(&philo->print_lock);
		printf("\033[1;33m%ld %zu will die in %ldms!\033[0m\n",
			get_time_since_start(philo), i + 1, time_left);
		philo->messenger_state = 1;
		pthread_mutex_unlock(&philo->print_lock);
	}
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
