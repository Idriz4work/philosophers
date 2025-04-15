/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 01:36:29 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/15 00:56:57 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <pthread.h>
#include <sys/time.h>
#include <string.h>

#define IS_THINKING -14
#define IS_DEAD -15
#define IS_ALIVE -18
#define IS_OVER -20

// dont need mutexes inside attribtues, the mutexes needs
// to be accesable for all therads therefore outside of them
typedef struct s_character
{
	int						log_permission;
	size_t					id;
	time_t					time_die;
	time_t					time_eat;
	time_t					time_sleep;
	int						state;
	size_t					right_chop;
	size_t					left_chop;
	size_t					dead;
	size_t					dinner_count;
	time_t					last_meal_time;
	size_t					thinking_start_time;
	size_t					time_think;
	size_t					sleeping_total;
	pthread_t				thread;
	struct s_philosophers	*parent;
}							t_attr;

// mutexes are located here so that all therads can access them
typedef struct s_philosophers
{
	int						messenger_state;
	int						someone_died;
	int						simulation_end;
	t_attr					*phil;
	size_t					n_philo;
	time_t					time_die;
	time_t					time_eat;
	time_t					time_sleep;
	size_t					dinner_count;
	size_t					dinner_number;
	pthread_mutex_t			chop_sticks[250];
	pthread_mutex_t			dead_lock;
	pthread_mutex_t			meal_lock;
	pthread_mutex_t			print_lock;
	pthread_mutex_t			state_lock;
	pthread_mutex_t			simulation_lock;
	pthread_mutex_t			simulation_end_lock;
	pthread_mutex_t			think_start_lock;
	pthread_mutex_t			change_simulation;
	pthread_mutex_t			think_lock;
	pthread_mutex_t			dinner_lock;
	// philo lock
	time_t					time_passed;
	time_t					start_time;
	pthread_t				*katil;
	int						count_meals;

}							t_philo;

//*********************************************************************** */
//** -------------------------------- FUNCTIONS ------------------------ */
//** ********************************************************************/

//
// INput parsing
int							philo_parser(int n_philos, char **argv,
								t_philo *philos, int argc);
int							input_parser(int argc, char **argv,
								t_philo *philos);
int							init_katil(t_philo *philo);
void						check_messenger(t_philo *philo, size_t i);
// main philo
void						*philosopher_one(t_philo *philo, size_t i);
void						*philosopher_algo(void *arg);

// init
void						time_log(t_philo *philo, size_t id, char action);
int							endtimes(size_t i, t_philo *philo);
void						mutex_cleanup(t_philo *philo, size_t n_philo);
int							init_fork_mutex(t_philo *philos);
int							init_join_threads(t_philo *philos, int n_philos);
int							init_threads(t_philo *philos, int n_philos);
int							init_philo(t_philo *philos, int n_philos);

// UTILS
int							ft_atoi(const char *s);
int							ft_isdigit(char *ce);
size_t						ft_strlen(const char *s);

// ACTIVITIES
void						think(size_t i, t_philo *philo);
void						ft_sleep_own(size_t times);
int							attend_to_eat_sleep(t_philo *philo, size_t i);
int							check_for_dinner(t_philo *philos);
void						pick_forks(t_philo *philo, size_t i);
void						release_forks(t_philo *philo, size_t i);

// activity checkers
int							has_simulation_ended(t_philo *phil);
void						*monitor_philosophers(void *arg);
void						starting_termination(t_philo *philosopher);

int							time_check(t_philo *philo, size_t i);
int							check_for_dinner(t_philo *philosophers);

// time functions
int							death_rules(t_philo *philo, size_t i);
time_t						elapsed_time_monitor(t_philo *philo, size_t i,
								time_t prev_t);
time_t						get_time_in_ms(void);
time_t						time_since_last_meal(t_philo *philo, size_t i);
int							is_just(size_t i);
time_t						get_time_since_start(t_philo *philo);
