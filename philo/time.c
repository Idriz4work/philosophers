/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:47:07 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/14 19:21:59 by iatilla-         ###   ########.fr       */
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
	time_t	res;

	res = 0;
	pthread_mutex_lock(&philo->meal_lock);
	cr_time = get_time_in_ms();
	res = cr_time - philo->phil[i].last_meal_time;
	pthread_mutex_unlock(&philo->meal_lock);
	return (res);
}

// this function specifically calculates the time that passed - current time
time_t	get_time_since_start(t_philo *philo)
{
	time_t	tv;

	tv = get_time_in_ms();
	return (tv - philo->start_time);
}
