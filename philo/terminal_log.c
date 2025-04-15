/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_log.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 19:22:10 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/15 01:02:48 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Modified status_print function
void	status_print(t_philo *philo, size_t id, char action,
		size_t elapsed_time)
{
	char	*str;

	if (action == 't')
		str = "is thinking";
	else if (action == 'f')
		str = "has taken a fork";
	else if (action == 'e')
		str = "is eating";
	else if (action == 's')
		str = "is sleeping";
	else if (action == 'd')
	{
		str = "died";
		printf("\033[0;31m%ld %zu %s\033[0m\n", elapsed_time, id + 1, str);
		return ;
	}
	else if (action == 'X')
		str = "putting down to table";
	else if (action == 'M')
		str = "philosopher will die in 10ms";
	pthread_mutex_lock(&philo->simulation_lock);
	if (philo->simulation_end != IS_OVER)
		printf("%ld %zu %s\n", elapsed_time, id + 1, str);
	pthread_mutex_unlock(&philo->simulation_lock);
}

// Modified time_log function
void	time_log(t_philo *philo, size_t id, char action)
{
	long	elapsed_time;
	int		simulation_over;

	pthread_mutex_lock(&philo->simulation_lock);
	simulation_over = (philo->simulation_end == IS_OVER);
	pthread_mutex_unlock(&philo->simulation_lock);
	if (simulation_over && action != 'd')
		return ;
	pthread_mutex_lock(&philo->print_lock);
	elapsed_time = get_time_in_ms() - philo->start_time;
	status_print(philo, id, action, elapsed_time);
	pthread_mutex_unlock(&philo->print_lock);
}
