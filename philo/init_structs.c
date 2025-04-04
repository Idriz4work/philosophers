/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:35:35 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/04 18:18:26 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// for each philo we have a mutex which is required for the
// Monitoring which will check the state of the philo isolated
int	mutex_each_philo(t_philo *philos)
{
	int	i;
	int	j;

	j = -1;
	i = 0;
	while (i < philos->n_philo)
	{
		if (pthread_mutex_init(&philos->philo_lock[i], NULL) == -1)
		{
			while (++j < i)
				pthread_mutex_destroy(&philos->philo_lock[j]);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

// Then initialize global mutexes
int	init_mutexer(t_philo *philos)
{
	int	i;
	int	j;

	j = -1;
	i = 0;
	if (mutex_each_philo(philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while (i < 250)
	{
		if (pthread_mutex_init(&philos->chop_sticks[i], NULL) == -1)
		{
			while (++j < i)
				pthread_mutex_destroy(&philos->chop_sticks[j]);
			return (EXIT_FAILURE);
		}
		i++;
	}
	if (pthread_mutex_init(&philos->meal_lock, NULL) == -1
	    || pthread_mutex_init(&philos->dead_lock, NULL) == -1
		|| pthread_mutex_init(&philos->print_lock, NULL) == -1)
	{
		free(philos->phil);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// set up the forks for the philosophers
// set up the right index number for the left & right fork
void	fork_setter(t_philo *philos, int i)
{
	int	n_philos;

	n_philos = philos->n_philo;
	philos->phil[i].left_chop = i;
	philos->phil[i].right_chop = (i + 1) % n_philos;
	if (i % 2)
	{
		philos->phil[i].right_chop = i;
		philos->phil[i].left_chop = (i + 1) % n_philos;
	}
}

// initialize the philospher attributes and also make a new thread
int	init_philo(t_philo *philos, int n_philos)
{
	int	i;

	i = 0;
	while (i < n_philos)
	{
		philos->phil[i].dead = 0;
		philos->phil[i].state = IS_THINKING;
		philos->simulation_end = IS_RUNNING;
		philos->someone_died = IS_ALIVE;
		philos->phil[i].log_permission = 0;
		if (philos->dinner_number != -1)
			philos->phil[i].dinner_count = 0;
		philos->phil[i].number_p = i;
		philos->phil[i].time_sleep = philos->time_sleep;
		philos->phil[i].time_die = philos->time_die;
		philos->phil[i].time_eat = philos->time_eat;
		philos->phil[i].parent = philos;
		philos->phil[i].last_meal_time = philos->start_time;
		fork_setter(philos, i);
		if (pthread_create(philos->phil[i].threads, NULL, &philosopher_algo,
				(void *)&philos->phil[i]) == -1)
		{
			printf("Error init threads for philosopher %d\n", i);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

// Allocate memory for the array of t_attr (one for each philosopher)
int	init_mem_philo(t_philo *philos, int n_philos)
{
	int	i;
	int	j;

	i = 0;
	philos->phil = (t_attr *)malloc(sizeof(t_attr) * n_philos);
	if (!philos->phil)
		return (EXIT_FAILURE);
	philos->katil = malloc(sizeof(pthread_t));
	if (!philos->katil)
		return (EXIT_FAILURE);
	while (i < n_philos)
	{
		philos->phil[i].threads = malloc(sizeof(pthread_t));
		if (!philos->phil[i].threads)
		{
			j = 0;
			while (j < i)
			{
				free(philos->phil[j].threads);
				j++;
			}
			free(philos->phil);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

// run the threads via join function
int	init_join_threads(t_philo *philos, int n_philos)
{
	int	i;

	i = 0;
	if (pthread_join(*philos->katil, NULL) == -1)
	{
		printf("Error JOINING threads for katil %d\n", i);
		return (EXIT_FAILURE);
	}
	while (i < n_philos)
	{
		if (pthread_join(*philos->phil[i].threads, NULL) == -1)
		{
			printf("Error JOINING threads for philosopher %d\n", i);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
