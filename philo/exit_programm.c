/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_programm.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:46:39 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/04 12:09:29 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Destroy mutexes && also make sure that everyhting is freed
void	mutex_cleanup(t_philo *philo, size_t n_philo)
{
	size_t		i;

	i = 0;
	while (i < n_philo)
	{
		pthread_mutex_destroy(&philo->chop_sticks[i]);
		pthread_mutex_destroy(&philo->philo_lock[i]);
		i++;
	}
	pthread_mutex_destroy(&philo->print_lock);
	pthread_mutex_destroy(&philo->dead_lock);
	pthread_mutex_destroy(&philo->meal_lock);
}

// function that makes sure that the programm
// exits and frees everything
int	endtimes(size_t i, t_philo *philo)
{
	pthread_mutex_lock(&philo->dead_lock);
	if (philo->phil[i].log_permission != -1)
		time_log(philo, i, 'd');
	pthread_mutex_unlock(&philo->dead_lock);
	mutex_cleanup(philo, philo->n_philo);
	i = 0;
	while (i < philo->n_philo)
	{
		free(philo->phil[i].threads);
		i++;
	}
	return (EXIT_SUCCESS);
}

// 1. go theou routine
// 2. if someone dies make sure that he finishes routine but not printing anything except that he died
// -> make sure that for every philosopher you set the state to IS_DEAD after 1 dies
// -> when every philosopher finished the routine no mutex is locked tehrefore no need for mutex_unlocker
// 3. in the main function have a endtimes function 
// 4. free everything and terminate programm
