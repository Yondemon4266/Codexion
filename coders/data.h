#ifndef __DATA_H
# define __DATA_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>


# define SUCCESS 0
# define ERROR 1

typedef struct s_data
{
    int nb_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    char    *scheduler;

} t_data;


int fill_data_struct(t_data *data, char **av);
void    print_data_structure(t_data *data);
void    init_data_struct(t_data *data);

#endif