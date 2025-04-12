/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 01:36:20 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/12 18:49:54 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// sleep function for accurecy
void	ft_sleep_own(size_t milliseconds)
{
	size_t	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < milliseconds)
		usleep(1);
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
	philo->someone_died = IS_DEAD;
	while (i < philo->n_philo)
	{
		philo->phil[i].state = IS_DEAD;
		i++;
	}
	philo->simulation_end = IS_OVER;
}

// Special case for a single philosopher
// actualy take the mutex & then release
void	*philosopher_one(t_philo *philo, size_t i)
{
	size_t	start;

	time_log(philo, i, 'f');
	pthread_mutex_lock(&philo->chop_sticks[philo->phil[i].left_chop]);
	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < philo->time_die)
	{
		usleep(100);
		check_messenger(philo, i);
	}
	pthread_mutex_unlock(&philo->chop_sticks[philo->phil[i].left_chop]);
	time_log(philo, i, 'd');
	endtimes(i,philo);
	return (NULL);
}

// regular routine until starvation
// i % 2 == 0 delay for just threads to wait until unjust finished eating
void	routine_multiple(t_attr *philosopher, size_t i)
{
	t_philo	*phil;

	phil = philosopher->parent;
	if (i % 2 == 1)
		ft_sleep_own(philosopher->time_eat);
	while (phil->simulation_end != IS_OVER)
	{
		attend_to_eat(phil, i);
		sleep_routine(phil, i);
		think(i, phil);
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
	if(philosopher->parent->simulation_end != IS_OVER)
	{
		if (i % 2 == 0)
			usleep(1000);
		if (philosopher->parent->n_philo == 1)
			return (philosopher_one(philosopher->parent, i));
		else
			routine_multiple(philosopher, i);
	}
	return (NULL);
}

// Initialize threads and values
// Create and start the monitor thread AFTER all philosopher threads
// This should run AFTER katil has completed
int	init_threads(t_philo *philos, int n_philos)
{
	philos->n_philo = n_philos;
	philos->start_time = get_time_in_ms();
	philos->time_passed = philos->start_time;
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
	if (ft_atoi(argv[1]) > 1)
	{
		if (init_threads(&phil_array, phil_array.n_philo) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
