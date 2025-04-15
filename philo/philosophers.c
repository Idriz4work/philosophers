/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 01:36:20 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/15 00:58:19 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Special case for a single philosopher
void	*philosopher_one(t_philo *philo, size_t i)
{
	size_t	start;

	time_log(philo, i, 'f');
	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < philo->time_die)
	{
		usleep(100);
		check_messenger(philo, i);
		if (philo->messenger_state == 1)
		{
			usleep(1000);
			printf("\033[0;31m%ld %zu died\033[0m\n", philo->time_die, i + 1);
			break ;
		}
	}
	return (NULL);
}

// checks carfeully if simulation has stopped
int	has_simulation_ended(t_philo *phil)
{
	int	r;

	r = EXIT_FAILURE;
	pthread_mutex_lock(&phil->simulation_lock);
	if (phil->simulation_end == IS_OVER)
	{
		r = EXIT_SUCCESS;
	}
	pthread_mutex_unlock(&phil->simulation_lock);
	return (r);
}

// Needs to die exactly at the time of die, meaning we dont wait
// until they finish eating or thinking we finish exactly when philosopher dies
void	*philosopher_algo(void *arg)
{
	t_attr	*philosopher;
	int		i;
	t_philo	*phil;

	philosopher = (t_attr *)arg;
	i = philosopher->id;
	if (i % 2 == 0)
		ft_sleep_own(philosopher->time_eat);
	if (philosopher->parent->n_philo == 1)
		return (philosopher_one(philosopher->parent, i));
	phil = philosopher->parent;
	while (has_simulation_ended(phil) == EXIT_FAILURE)
	{
		attend_to_eat_sleep(phil, i);
		think(i, phil);
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
	philos->phil = (t_attr *)malloc(sizeof(t_attr) * n_philos);
	if (!philos->phil)
		return (EXIT_FAILURE);
	philos->katil = malloc(sizeof(pthread_t));
	if (!philos->katil)
	{
		free(philos->phil);
		return (EXIT_FAILURE);
	}
	if (init_fork_mutex(philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_philo(philos, n_philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_katil(philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_join_threads(philos, n_philos) == EXIT_FAILURE)
	{
		endtimes(0, philos);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

//
int	main(int argc, char **argv)
{
	t_philo	phil_array;

	memset(&phil_array, 0, sizeof(t_philo));
	if (input_parser(argc, argv, &phil_array) == EXIT_FAILURE)
	{
		printf("ERROR while parsing input\n");
		return (EXIT_FAILURE);
	}
	if (ft_atoi(argv[1]) > 1)
	{
		if (init_threads(&phil_array, phil_array.n_philo) == EXIT_FAILURE)
		{
			printf("Init threads failed\n");
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
