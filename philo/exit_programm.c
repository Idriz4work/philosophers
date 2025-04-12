/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_programm.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:46:39 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/12 18:09:26 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
}

// function that makes sure that the program
// exits and frees everything
int endtimes(size_t i, t_philo *philo)
{
    mutex_cleanup(philo, philo->n_philo);
    free(philo->katil); 
    free(philo->phil);
	// int j = 0;
	// while (j < philo->n_philo)
	// {
	// 	free(philo->phil[j].thread);
	// 	j++;
	// }
    return (EXIT_SUCCESS);
}
// 1. go theou routine
// 2. if someone dies make sure that he finishes routine but not printing anything except that he died
// -> make sure that for every philosopher you set the state to IS_DEAD after 1 dies
// -> when every philosopher finished the routine no mutex is locked tehrefore no need for mutex_unlocker
// 3. in the main function have a endtimes function 
// 4. free everything and terminate programm
