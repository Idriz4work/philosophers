/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_programm.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:46:39 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/15 01:02:31 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Define the starting_termination function to properly set death states
// In exit_programm.c, modify starting_termination to use simulation_lock:
void	starting_termination(t_philo *philo)
{
	pthread_mutex_lock(&philo->simulation_lock);
	philo->simulation_end = IS_OVER;
	pthread_mutex_unlock(&philo->simulation_lock);
}

// Destroy mutexes && also make sure that everything is freed
void	mutex_cleanup(t_philo *philo, size_t n_philo)
{
	size_t	i;

	i = 0;
	while (i < n_philo)
	{
		pthread_mutex_destroy(&philo->chop_sticks[i]);
		i++;
	}
	pthread_mutex_destroy(&philo->print_lock);
	pthread_mutex_destroy(&philo->dead_lock);
	pthread_mutex_destroy(&philo->meal_lock);
	pthread_mutex_destroy(&philo->state_lock);
	pthread_mutex_destroy(&philo->change_simulation);
	pthread_mutex_destroy(&philo->think_lock);
	pthread_mutex_destroy(&philo->think_start_lock);
	pthread_mutex_destroy(&philo->dinner_lock);
	pthread_mutex_destroy(&philo->simulation_end_lock);
	pthread_mutex_destroy(&philo->simulation_lock);
}

// function that makes sure that the program
// exits and frees everything
int	endtimes(size_t i, t_philo *philo)
{
	mutex_cleanup(philo, philo->n_philo);
	free(philo->katil);
	free(philo->phil);
	return (EXIT_SUCCESS);
}
