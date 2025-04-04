/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 01:36:20 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/04 18:20:39 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// this f
void	sleep_own(size_t i, t_philo *philo, char action)
{
	time_t	think;

	think = (philo->time_die - (get_time_in_ms()
				- philo->phil[i].last_meal_time) - philo->time_eat) / 2;
	if (think < 0)
		think = 0;
	if (think > 600)
		think = 200;
	if (action == 's')
	{
		time_log(philo, i, 's');
		usleep(philo->phil[i].time_sleep * 1000);
	}
	else if (action == 'e')
		usleep(philo->phil[i].time_eat * 1000);
	else if (action == 't')
		usleep(think * 1000);
}

// this is the start of the terminator which will make sure that the
// philosophers have no rights to print messages to the terminal and
// making sure that they finish the routine until endtime are called
// Function that marks all philosophers as dead to terminate the simulation
// Define the starting_termination function to properly set death states
void	starting_termination(t_philo *philo)
{
	size_t	i;

	i = 0;
	// Set the global death flag
	philo->someone_died = IS_DEAD;
	// Set each philosopher's state to dead
	while (i < philo->n_philo)
	{
		philo->phil[i].state = IS_DEAD;
		i++;
	}
	// Also set simulation end flag
	philo->simulation_end = IS_OVER;
}

// Special case for a single philosopher
int	philosopher_one(t_philo *philo, size_t i)
{
	think(i, philo);
	while (philo->phil[i].state != IS_DEAD)
		usleep(1000);
	return (EXIT_SUCCESS);
}

// main routine for philosophers, without number of meals given
void	routine_multiple_meals(t_attr *philosopher, size_t i)
{
	int	j;

	j = 0;
	while (philosopher->state != IS_DEAD
		&& j < philosopher->parent->dinner_number)
	{
		think(i, philosopher->parent);
		attend_to_eat(philosopher->parent, i);
		sleep_own(i, philosopher->parent, 's');
		j++;
	}
	if (j == philosopher->parent->dinner_number)
		return ;
	else
		printf("time to die is: %i\n", philosopher->time_die);
}

// main routine for philosophers, without number of meals given
void routine_multiple(t_attr *philosopher, size_t i)
{
    while (1)
    {
        pthread_mutex_lock(&philosopher->parent->dead_lock);
        if (philosopher->state == IS_DEAD || 
            philosopher->parent->someone_died == IS_DEAD ||
            philosopher->parent->simulation_end == IS_OVER)
        {
            pthread_mutex_unlock(&philosopher->parent->dead_lock);
            break;
        }
        pthread_mutex_unlock(&philosopher->parent->dead_lock);
        
        think(i, philosopher->parent);
        
        // Check death status after each operation
        pthread_mutex_lock(&philosopher->parent->dead_lock);
        if (philosopher->state == IS_DEAD || 
            philosopher->parent->someone_died == IS_DEAD ||
            philosopher->parent->simulation_end == IS_OVER)
        {
            pthread_mutex_unlock(&philosopher->parent->dead_lock);
            break;
        }
        pthread_mutex_unlock(&philosopher->parent->dead_lock);
        
        attend_to_eat(philosopher->parent, i);
        
        // Check again
        pthread_mutex_lock(&philosopher->parent->dead_lock);
        if (philosopher->state == IS_DEAD || 
            philosopher->parent->someone_died == IS_DEAD ||
            philosopher->parent->simulation_end == IS_OVER)
        {
            pthread_mutex_unlock(&philosopher->parent->dead_lock);
            break;
        }
        pthread_mutex_unlock(&philosopher->parent->dead_lock);
        
        sleep_own(i, philosopher->parent, 's');
    }
}

// Needs to die exactly at the time of die, meaning we dont wait
// until they finish eating or thinking we finish exactly when philosopher dies
void	*philosopher_algo(void *arg)
{
	t_attr	*philosopher;
	int		i;

	philosopher = (t_attr *)arg;
	i = philosopher->number_p;
	if (i % 2 == 0)
		usleep(1000);
	if (philosopher->parent->n_philo == 1)
		philosopher_one(philosopher->parent, i);
	else if (philosopher->dinner_count == -1)
		routine_multiple_meals(philosopher, i);
	else
		routine_multiple(philosopher, i);
	endtimes(i, philosopher->parent);
	return (NULL);
}

// Initialize threads and values
// Create and start the monitor thread AFTER all philosopher threads
// This should run AFTER katil has completed
int	init_threads(t_philo *philos, int n_philos)
{
	philos->n_philo = n_philos;
	philos->time_passed = 0;
	philos->start_time = get_time_in_ms();
	if (init_mem_philo(philos, n_philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_mutexer(philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_philo(philos, n_philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_katil(philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_join_threads(philos, n_philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

//
int	main(int argc, char **argv)
{
	t_philo	phil_array;

	if (input_parser(argc, argv, &phil_array) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_threads(&phil_array, phil_array.n_philo) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
