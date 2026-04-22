#ifndef __DATA_H
# define __DATA_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define SUCCESS 0
# define ERROR 1

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				released_time;
	int				*heap[2];

}					t_dongle;

typedef struct s_coder
{
	pthread_t		coder;
	int				id;
	int				times_compiled;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
}					t_coder;

typedef struct s_data
{
	int				nb_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	char			*scheduler;
	t_coder			*coders;
	t_dongle		*dongles;

}					t_data;

void				print_data_structure(t_data *data);
void				print_error_arguments(void);
int					init_all_data(t_data *data, int ac, char **av);
int					fill_dongles_coders(t_data *data);
int					parse_data(t_data *data, char **av);
void				free_dongles(t_dongle *dongles, int dongle_len);

#endif