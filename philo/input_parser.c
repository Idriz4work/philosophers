/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:40:26 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/14 19:20:23 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Initialize basic values
// Initialize mutexes and threads after basic initialization
int	philo_parser(int n_philos, char **argv, t_philo *philos, int argc)
{
	int	meal_count;

	if (ft_atoi(argv[2]) > 0)
		philos->time_die = ft_atoi(argv[2]);
	if (ft_atoi(argv[3]) >= 0)
		philos->time_eat = ft_atoi(argv[3]);
	if (ft_atoi(argv[4]) > 0)
		philos->time_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		if (!ft_isdigit(argv[5]))
			return (EXIT_FAILURE);
		meal_count = ft_atoi(argv[5]);
		if (meal_count < 0)
			return (EXIT_FAILURE);
		philos->count_meals = ft_atoi(argv[5]);
	}
	else
		philos->count_meals = -1;
	if (init_threads(philos, n_philos) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

// Set all philosophers with the same initial values
// meals can be 0 as well higher than that
// dont test with more than 200 philos
int	input_parser(int argc, char **argv, t_philo *philos)
{
	int	n_philos;

	if (argc < 5 || argc > 6)
		return (EXIT_FAILURE);
	if (ft_isdigit(argv[1]) && ft_isdigit(argv[2]) && ft_isdigit(argv[3])
		&& ft_isdigit(argv[4]))
	{
		n_philos = ft_atoi(argv[1]);
		if (n_philos <= 0 || n_philos > 200)
		{
			printf("MIN:1+/MAX:200. 200+ philosophers is not allowed\n");
			return (EXIT_FAILURE);
		}
		philos->n_philo = n_philos;
		philos->messenger_state = 0;
		return (philo_parser(n_philos, argv, philos, argc));
	}
	return (EXIT_FAILURE);
}
