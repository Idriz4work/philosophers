/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:35:35 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/12 18:18:58 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Then initialize global mutexes
int	init_mutexer(t_philo *philos)
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
	usleep(1000);
	while (i < n_philos)
	{
		philos->phil[i].thinking_start_time = 0;
		philos->phil[i].total_thinking_time = 0;
		philos->phil[i].dead = 0;
		philos->phil[i].state = IS_THINKING;
		philos->simulation_end = IS_ALIVE;
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

// In init_mem_philo (this part is correct as is):
int	init_mem_philo(t_philo *philos, int n_philos)
{
	philos->phil = (t_attr *)malloc(sizeof(t_attr) * n_philos);
	if (!philos->phil)
		return (EXIT_FAILURE);
	philos->katil = malloc(sizeof(pthread_t));
	if (!philos->katil)
	{
		free(philos->phil);
		return (EXIT_FAILURE);
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
	if (pthread_join(*philos->katil, NULL) != 0) // Note the dereference with *
	{
		printf("Error JOINING threads for katil\n");
		return (EXIT_FAILURE);
	}
	while (i < n_philos)
	{
		if (pthread_join(philos->phil[i].thread, NULL) != 0)
		{
			printf("Error JOINING threads for philosopher %d\n", i);
			return (EXIT_FAILURE);
		}
		i++;
	}
	// while(1)
	// {
	// 	if (death_rules(philos, i) == EXIT_FAILURE)
	// 	{
	// 		return(EXIT_FAILURE);
	// 	}
	// }
	endtimes(i,philos);
	return (EXIT_SUCCESS);
}
