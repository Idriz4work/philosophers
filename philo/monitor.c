/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:46:57 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/15 01:06:39 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// rules that determine to die
// 1. Late eating
int	death_rules(t_philo *philo, size_t i)
{
	time_t	lst_meal_time;

	lst_meal_time = time_since_last_meal(philo, i);
	if (lst_meal_time >= philo->time_die)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

//
// Modified oberserver_ktl function
int	oberserver_ktl(t_philo *philo, size_t i)
{
	size_t	elapsed_time;

	while (i < philo->n_philo)
	{
		check_messenger(philo, i);
		if (death_rules(philo, i) == EXIT_FAILURE)
		{
			pthread_mutex_lock(&philo->simulation_end_lock);
			if (philo->simulation_end != IS_OVER)
			{
				starting_termination(philo);
				elapsed_time = get_time_in_ms() - philo->start_time;
				printf("\033[0;31m%ld %zu died\033[0m\n", elapsed_time, i + 1);
			}
			pthread_mutex_unlock(&philo->simulation_end_lock);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	dinner_break(t_philo *philo, int needs_meals)
{
	if (philo->count_meals != -1 && needs_meals == 1)
	{
		pthread_mutex_lock(&philo->simulation_end_lock);
		starting_termination(philo);
		pthread_mutex_unlock(&philo->simulation_end_lock);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

// Modified monitor function to properly handle single philosopher case
// Check if time limit is exceeded
void	*monitor_philosophers(void *arg)
{
	t_philo	*philo;
	size_t	i;
	int		needs_meals;

	needs_meals = -1;
	philo = (t_philo *)arg;
	usleep(1000);
	if (philo->n_philo == 1)
		return (NULL);
	while (has_simulation_ended(philo) != EXIT_SUCCESS)
	{
		i = 0;
		if (philo->count_meals != -1)
			if (check_for_dinner(philo) == EXIT_SUCCESS)
				needs_meals = 1;
		if (oberserver_ktl(philo, i) == EXIT_FAILURE)
			return (NULL);
		if (dinner_break(philo, needs_meals) == EXIT_SUCCESS)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

// In init_katil:
int	init_katil(t_philo *philo)
{
	usleep(1000);
	if (philo->n_philo >= 1)
	{
		if (pthread_create(philo->katil, NULL, monitor_philosophers,
				philo) != 0)
		{
			printf("Error creating monitor thread\n");
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
