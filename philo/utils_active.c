/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_active.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:23:52 by iatilla-          #+#    #+#             */
/*   Updated: 2025/03/19 17:44:03 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


// Improved check_meal_time function
// int check_meal_time(t_attr *philosopher, size_t i)
// {
//     philosopher->last_meal_time = time_since_last_meal(philosopher->parent, i);
//     if (philosopher->last_meal_time > philosopher->time_die)
//     {
//         pthread_mutex_lock(&philosopher->parent->dead_lock);
//         philosopher->state = IS_DEAD;
//         philosopher->parent->simulation_end = 1; // Flag to end simulation
//         pthread_mutex_unlock(&philosopher->parent->dead_lock);
        
//         time_log(philosopher->parent, i, 'd');
//         return (EXIT_FAILURE);
//     }
//     return (EXIT_SUCCESS);
// }

// // Improved is_alive function
// int is_alive(t_philo *philo, size_t i)
// {
//     int result;

//     result = EXIT_SUCCESS;
//     pthread_mutex_lock(&philo->dead_lock);
//     if (philo->phil[i].state == IS_DEAD || philo->simulation_end)
//         result = EXIT_FAILURE;
//     pthread_mutex_unlock(&philo->dead_lock);
    
//     return (result);
// }

// check if the time thought was lonegr than the time to die
int	check_meal_time(t_attr *philosopher, size_t i)
{
	philosopher->last_meal_time = time_since_last_meal(philosopher->parent, i);
	if (philosopher->last_meal_time > philosopher->time_die)
	{
		time_log(philosopher->parent,i,'d');
		// endtimes(i, philosopher->parent);
		// return (EXIT_FAILURE);
		;
	}
	return (EXIT_SUCCESS);
}

//
int	is_alive(t_philo *philo, size_t i)
{
	int	result;

	result = EXIT_SUCCESS;
	pthread_mutex_lock(&philo->dead_lock);
	if (philo->phil[i].state == IS_DEAD)
		result = EXIT_FAILURE;
	pthread_mutex_unlock(&philo->dead_lock);
	if (result == EXIT_FAILURE)
		endtimes(i, philo);
	return (result);
}

// checks if the number of eating cycles was reached
int	check_dinner(t_philo *philosopher)
{
	if (philosopher->dinner_count == philosopher->dinner_number)
		return (EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
