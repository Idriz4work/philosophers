/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 01:36:20 by iatilla-          #+#    #+#             */
/*   Updated: 2025/03/19 16:36:45 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// this f
void	sleep_own(size_t i, t_philo *philo, char action)
{
    time_t think;
	pthread_mutex_lock(&philo->last_meal_lock);
	think = (philo->time_die - (get_time_in_ms() - philo->phil[i].last_meal_time) - philo->time_eat) / 2;
    pthread_mutex_unlock(&philo->last_meal_lock);
    if (think < 0)
        think = 0;
    if (think < 0)
        think = 1;
    if (think > 600)
        think = 200;
    // if (new == false)
    //     report(THOUGHTS, philo);



	
	if (action == 's')
	{
		time_log(philo,i,'s');
		usleep(philo->phil[i].time_sleep * 1000);
	}
	else if (action == 'e')
		usleep(philo->phil[i].time_eat * 1000);
	else if (action == 't')
		usleep(think * 1000);
}

int	philosopher_one(t_philo *philo, size_t i)
{
	while (1)
	{
		think(i, philo);
		if (is_alive(philo, i) == EXIT_FAILURE)
		{
			endtimes(i, philo);
			return (EXIT_SUCCESS);
		}
	}
	return (EXIT_FAILURE);
}

// calls routine tepending on number of args
// Think first
// Then try to eat
// Then sleep
void	*philosopher_algo(void *arg)
{
	t_attr	*philosopher;
	int		i;

	philosopher = (t_attr *)arg;
	i = philosopher->number_p;
	if (i % 2 == 1)
		usleep(1000);
	if (philosopher->parent->n_philo == 0)
		if (philosopher_one(philosopher->parent, i) == EXIT_SUCCESS)
			return (NULL);
	while (is_alive(philosopher->parent, i) != EXIT_FAILURE)
	{
		think(i, philosopher->parent);
		if (check_meal_time(philosopher, i) == EXIT_FAILURE)
			return (NULL);
		attend_to_eat(philosopher->parent, i);
		sleep_own(i, philosopher->parent, 's');
		philosopher->parent->dinner_count++;
	}
	return (NULL);
}

// Initialize threads and values
// join threads
int	init_threads(t_philo *philos, int n_philos)
{
	int	i;

	i = 0;
	philos->n_philo = n_philos;
	philos->time_passed = 0;
	philos->start_time = get_time_in_ms();
	if (init_mem_philo(philos, n_philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_mutexer(philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_philo(philos, n_philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_join_threads(philos, n_philos) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

//
int	main(int argc, char **argv)
{
	t_philo		phil_array;
	static int	simulation = IS_ALIVE;

	if (input_parser(argc, argv, &phil_array) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	phil_array.time_passed = 0;
	if (init_threads(&phil_array, phil_array.n_philo) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
