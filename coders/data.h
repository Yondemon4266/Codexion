#ifndef __DATA_H
# define __DATA_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

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


long    ft_atoi(char *s);

#endif