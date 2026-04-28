/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 14:37:40 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/28 16:35:16 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __DATA_H
# define __DATA_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define SUCCESS 0
# define ERROR 1
# define BURNED_OUT 2
# define ALL_COMPILED 3
# define COMPILING 4

typedef struct s_coder		t_coder;

typedef struct s_data		t_data;

typedef struct s_init_flags	t_init_flags;

enum						e_scheduler
{
	MODE_FIFO,
	MODE_EDF
};

typedef struct s_dongle
{
	pthread_mutex_t			mutex;
	int						released_time;
	t_coder					*queue[2];

}							t_dongle;

struct						s_coder
{
	pthread_t				coder;
	int						id;
	int						times_compiled;
	int						is_compiling;
	long					last_compile_start;
	t_dongle				*left_dongle;
	t_dongle				*right_dongle;
	t_data					*data;
	struct timeval			time;
	pthread_mutex_t			coder_lock;
	pthread_cond_t			wait_compil_cond;
};

struct						s_init_flags
{
	int						start_init_flag;
	int						start_cond_init_flag;
	int						stop_init_flag;
	int						print_init_flag;
	int						coders_lock_flag;
	int						coders_cond_flag;
	int						dongles_flag;
};

typedef struct s_simulation_state
{
	int						sim_failed;
	int						sim_success;
	int						burned_out;
	int						count_created_threads;

}							t_simulation_state;

struct						s_data
{
	int						nb_coders;
	int						time_to_burnout;
	int						time_to_compile;
	int						time_to_debug;
	int						time_to_refactor;
	int						number_of_compiles_required;
	int						dongle_cooldown;
	int						start_simulation;
	int						stop_simulation;
	enum e_scheduler		scheduler;
	pthread_t				monitor;
	pthread_mutex_t			start_lock;
	pthread_cond_t			start_cond;
	pthread_mutex_t			stop_lock;
	pthread_mutex_t			print_lock;
	t_coder					*coders;
	t_dongle				*dongles;
	t_init_flags			init_flags;
	t_simulation_state		simulation_state;
	long					simulation_start_time;
};

/* ---------------------------------- INIT ---------------------------------- */
int							init_all_data(t_data *data, int ac, char **av);
int							parse_data(t_data *data, char **av);
int							fill_dongles_coders(t_data *data);

/* ------------------------------- SIMULATION ------------------------------- */
int							run_monitor(t_data *data);
void						stop_simulation(t_data *data);
void						stop_failed_simulation(t_data *data);

/* ---------------------------- MONITOR TRACKING ---------------------------- */
int							track_burnout(t_data *data);

/* ----------------------------- CODER ROUTINE ------------------------------ */
void						*routine_coder(void *arg);

/* ----------------------------- CODER ACTIONS ------------------------------ */
void						print_coder(t_coder *coder, char *s);
void						update_last_compile_start_time(t_coder *coder);
void						increment_compilation(t_coder *coder);
void						compiling(t_coder *coder);
void 						debugging(t_coder *coder);
void 						refactoring(t_coder *coder);


/* ----------------------------- QUEUE MANAGER ------------------------------ */
void						request_dongles(t_dongle *left, t_dongle *right,
								t_coder *coder, enum e_scheduler mode);
void						release_dongles(t_coder *coder);


/* -------------------------------- CLEANUP --------------------------------- */
void						cleanup_all(t_data *data);
void						free_coders_mutex(t_data *data, int offset);
void						free_coders_cond(t_data *data, int offset);
void						free_dongles_mutex(t_dongle *dongles, int offset);

/* --------------------------------- UTILS ---------------------------------- */
long long					get_current_time_ms(void);
int							init_time_of_coders(t_data *data);
int							check_simulation_status(t_data *data);
void						print_error_arguments(void);
void						print_data_structure(t_data *data);
#endif