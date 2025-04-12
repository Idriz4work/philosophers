/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:47:07 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/12 18:51:37 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Lock for both printing and checking simulation state
// Check simulation state first - exit early if simulation is over
void	time_log(t_philo *philo, size_t id, char action)
{
	char	*str;
	long	elapsed_time;
	int		should_print;

	should_print = 0;
	pthread_mutex_lock(&philo->print_lock);
	if (philo->simulation_end == IS_OVER)
	{
		pthread_mutex_unlock(&philo->print_lock);
		return ;
	}
	elapsed_time = get_time_in_ms() - philo->start_time;
	if (action == 't')
		str = "is thinking";
	else if (action == 'f')
		str = "has taken a fork";
	else if (action == 'e')
		str = "is eating";
	else if (action == 's')
		str = "is sleeping";
	else if (action == 'X')
		str = "putting down to table";
	else if (action == 'M')
		str = "philosopher will die in 10ms";
	if (philo->simulation_end != IS_OVER)
		printf("%ld %zu %s\n", elapsed_time, id + 1, str);
	pthread_mutex_unlock(&philo->print_lock);
}

// Get current time in milliseconds
time_t	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

// Calculate time elapsed since last meal in milliseconds
time_t	time_since_last_meal(t_philo *philo, size_t i)
{
	time_t	cr_time;

	cr_time = get_time_in_ms();
	return (cr_time - philo->phil[i].last_meal_time);
}

// this function specifically calculates the time that passed - current time
time_t	get_time_since_start(t_philo *philo)
{
	time_t	tv;

	tv = get_time_in_ms();
	return (tv - philo->start_time);
}
