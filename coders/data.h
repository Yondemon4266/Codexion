#ifndef __DATA_H
# define __DATA_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>

# define SUCCESS 0
# define ERROR 1


typedef struct s_coder t_coder;

typedef struct s_data t_data;

typedef struct s_init_flags t_init_flags;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				released_time;
	t_coder			*queue[2];

}					t_dongle;

struct s_coder
{
	pthread_t		coder;
	int				id;
	int				times_compiled;
	int				last_compile_start;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_data			*data;
	pthread_mutex_t	coder_lock;
};


struct s_init_flags
{
	int	start_init_flag;
	int	start_cond_init_flag;
	int	stop_init_flag;
	int	coders_lock_flag;

};

typedef struct s_simulation_state
{
	int	sim_failed;
	int	burned_out;
	int	count_created_threads;

} t_simulation_state;

struct s_data
{
	int				nb_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	char			*scheduler;
	pthread_t		monitor;
	int				start_simulation;
	pthread_mutex_t	start_lock;
	pthread_cond_t	start_cond;
	int				stop_simulation;
	pthread_mutex_t	stop_lock;
	t_coder			*coders;
	t_dongle		*dongles;
	t_init_flags	init_flags;
	t_simulation_state	simulation_state;
	struct timeval	simulation_start_time;
};




void				print_data_structure(t_data *data);
void				print_error_arguments(void);
int					init_all_data(t_data *data, int ac, char **av);
int					fill_dongles_coders(t_data *data);
int					parse_data(t_data *data, char **av);
void				cleanup_all(t_data *data);
int 				run_monitor(t_data *data);
void				*routine_coder(void *arg);
void				free_coders_mutex(t_data *data, int	offset);
int					check_simulation_status(t_data *data);


#endif