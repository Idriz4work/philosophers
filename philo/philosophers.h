/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatilla- <iatilla-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 01:36:29 by iatilla-          #+#    #+#             */
/*   Updated: 2025/04/12 18:09:18 by iatilla-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <pthread.h>
#include <sys/time.h>
// #include <time.h>

#define IS_THINKING -14
#define IS_DEAD -15
#define IS_ALIVE -18
#define IS_OVER -20

// you dony need mutexes inside your attribtues, the mutexes needs
// to be accesable for all therads therefore outside of them
typedef struct s_character
{
	int						log_permission;
	size_t					number_p;
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
	size_t					total_thinking_time;
	size_t					sleeping_total;
	pthread_t				thread;
	struct s_philosophers	*parent;
}							t_attr;

// pthread_mutex_t	mutex_dead;
// pthread_mutex_t	mutex_meal;
// pthread_mutex_t	mutex_eat;
// pthread_mutex_t	r_chop_block;
// pthread_mutex_t	l_chop_block;
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
	// philo lock
	time_t					time_passed;
	time_t					start_time;
	pthread_t				*katil;

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
int							init_mutexer(t_philo *philos);
int							init_philo(t_philo *philos, int n_philos);
int							init_mem_philo(t_philo *philos, int n_philos);
int							init_join_threads(t_philo *philos, int n_philos);
int							init_threads(t_philo *philos, int n_philos);

// UTILS
int							ft_atoi(const char *s);
int							ft_isdigit(char *ce);
size_t						ft_strlen(const char *s);

// ACTIVITIES
void						think(size_t i, t_philo *philo);
void						eat(size_t i, t_philo *philo);
void						ft_sleep_own(size_t times);
void						put_forks(size_t i, t_philo *philo);
void						eat(size_t i, t_philo *philo);
int							attend_to_eat(t_philo *philo, size_t i);
void						sleep_routine(t_philo *philo, size_t i);

// activity checkers
void						*monitor_philosophers(void *arg);
void						starting_termination(t_philo *philosopher);

int							time_check(t_philo *philo, size_t i);
int							check_dinner(t_philo *philosopher);

// time functions
int							death_rules(t_philo *philo, size_t i);
time_t						elapsed_time_monitor(t_philo *philo, size_t i,
								time_t prev_t);
time_t						get_time_in_ms(void);
time_t						time_since_last_meal(t_philo *philo, size_t i);
int							is_just(size_t i);
time_t						get_time_since_start(t_philo *philo);
