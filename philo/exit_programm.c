/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_programm.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:46:39 by iatilla-          #+#    #+#             */
/*   Updated: 2025/03/19 17:44:12 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"



// Destroy mutexes && also make sure that everyhting is freed
// Destroy only the actually initialized mutexes
void	mutex_cleanup(t_philo *philo, size_t n_philo)
{
	size_t		i;
	static int	cleaned_up = 0;

	// Prevent multiple cleanup calls
	// Destroy all chopstick mutexes
	if (cleaned_up)
		return ;
	cleaned_up = 1;
	i = 0;
	while (i < n_philo)
	{
		pthread_mutex_destroy(&philo->chop_sticks[i]);
		// Destroy global mutexes
		i++;
	}
	pthread_mutex_destroy(&philo->print_lock);
	pthread_mutex_destroy(&philo->dead_lock);
	pthread_mutex_destroy(&philo->meal_lock);
}


// makes sure that the mutexes are unlcoked before freeing
void mutexes_unlocker(t_philo *philo)
{
    int i;
    // int ret;

    i = 0;
	// while (i < )
}

// function that makes sure that the programm
// exits and frees everything
int	endtimes(size_t i, t_philo *philo)
{
	time_log(philo, i, 'd');
	pthread_mutex_lock(&philo->exit_lock);
	mutexes_unlocker(philo);
	mutex_cleanup(philo, philo->n_philo);
	i = 0;
	while (i < philo->n_philo)
	{
		free(philo->phil[i].threads);
		i++;
	}
	free(philo->phil);
	pthread_mutex_unlock(&philo->exit_lock);
	return (EXIT_SUCCESS);
}

/ // Improved mutex_cleanup function
// void mutex_cleanup(t_philo *philo, size_t n_philo)
// {
//     static int cleaned_up = 0;
//     size_t i;

//     if (cleaned_up)
//         return;
//     cleaned_up = 1;
    
//     for (i = 0; i < n_philo; i++)
//         pthread_mutex_destroy(&philo->chop_sticks[i]);
        
//     pthread_mutex_destroy(&philo->print_lock);
//     pthread_mutex_destroy(&philo->meal_lock);
//     pthread_mutex_destroy(&philo->dead_lock);
//     pthread_mutex_destroy(&philo->exit_lock);
// }

// // Improved mutexes_unlocker function
// void mutexes_unlocker(t_philo *philo)
// {
//     int i;

//     i = 0;
//     while (i < philo->n_philo)
//     {
//         // Try to unlock all forks - pthread_mutex_unlock is safe to call
//         // even if the mutex is not locked by the calling thread
//         pthread_mutex_unlock(&philo->chop_sticks[i]);
//         i++;
//     }
// }

// // Improved endtimes function
// int endtimes(size_t i, t_philo *philo)
// {
//     // Mark simulation as ended
//     pthread_mutex_lock(&philo->dead_lock);
//     philo->simulation_end = 1;
//     pthread_mutex_unlock(&philo->dead_lock);
    
//     // Only log death if this is the first philosopher to die
//     pthread_mutex_lock(&philo->print_lock);
//     if (!philo->death_logged) {
//         printf("%lu %lu died\n", get_time_in_ms() - philo->start_time, i + 1);
//         philo->death_logged = 1;
//     }
//     pthread_mutex_unlock(&philo->print_lock);
    
//     // No need to unlock and free here - should be done in main thread
//     // after all philosopher threads have joined
    
//     return (EXIT_SUCCESS);
// }


