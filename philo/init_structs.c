/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:35:35 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/14 19:29:46 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//
int	init_locks(t_philo *philos)
{
	if (pthread_mutex_init(&philos->meal_lock, NULL) == -1
		|| pthread_mutex_init(&philos->dead_lock, NULL) == -1
		|| pthread_mutex_init(&philos->think_start_lock, NULL) == -1
		|| pthread_mutex_init(&philos->think_lock, NULL) == -1
		|| pthread_mutex_init(&philos->dinner_lock, NULL) == -1
		|| pthread_mutex_init(&philos->change_simulation, NULL) == -1
		|| pthread_mutex_init(&philos->simulation_end_lock, NULL) == -1
		|| pthread_mutex_init(&philos->simulation_lock, NULL) == -1
		|| pthread_mutex_init(&philos->state_lock, NULL) == -1
		|| pthread_mutex_init(&philos->print_lock, NULL) == -1)
	{
		free(philos->phil);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// Then initialize global mutexes
int	init_fork_mutex(t_philo *philos)
{
	int	i;
	int	j;

	j = -1;
	i = 0;
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
	if (init_locks(philos) == EXIT_FAILURE)
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
		philos->phil[i].time_think = 0;
		philos->phil[i].state = IS_THINKING;
		if (philos->dinner_number != -1)
			philos->phil[i].dinner_count = 0;
		philos->phil[i].id = i;
		philos->phil[i].time_sleep = philos->time_sleep;
		philos->phil[i].time_die = philos->time_die;
		philos->phil[i].time_eat = philos->time_eat;
		philos->phil[i].parent = philos;
		philos->phil[i].last_meal_time = philos->start_time;
		fork_setter(philos, i);
		if (pthread_create(&philos->phil[i].thread, NULL, &philosopher_algo,
				(void *)&philos->phil[i]) != 0)
		{
			printf("Error init threads for philosopher %d\n", i);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

// In init_join_threads (fix the join call):
int	init_join_threads(t_philo *philos, int n_philos)
{
	int		i;
	t_attr	*philosoph;

	i = 0;
	philosoph = philos->phil;
	if (pthread_join(*philos->katil, NULL) != 0)
	{
		printf("Error JOINING threads for katil\n");
		return (EXIT_FAILURE);
	}
	while (i < n_philos)
	{
		if (pthread_join(philos->phil[i].thread, NULL) != 0)
		{
			printf("Error JOINING threads for philosopher %d\n", i);
			endtimes(i, philos);
			return (EXIT_FAILURE);
		}
		i++;
	}
	endtimes(i, philos);
	return (EXIT_SUCCESS);
}
