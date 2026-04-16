
#include "data.h"

void    init_data_struct(t_data *data)
{
    data->nb_coders = 0;
    data->time_to_burnout = 0;
    data->time_to_compile = 0;
    data->time_to_debug = 0;
    data->time_to_refactor = 0;
    data->number_of_compiles_required = 0;
    data->dongle_cooldown = 0;
    data->scheduler = NULL;
}


int check_integer(long number)
{
    if (number <= INT_MAX && number >= 0)
        return (EXIT_SUCCESS);
    return (EXIT_FAILURE);
}


int    fill_data_struct(t_data *data, char **av)
{
    long tmp;
    int i;

    i = 0;

    while (i < 7)
    {
        tmp = ft_atoi(av[i]);
        if (check_integer(tmp) == EXIT_FAILURE)
        {
            fprintf(stderr, "Error, time must be between INT_MAX and 0\n");
            return (EXIT_FAILURE);
        }
        if (i == 0) data->nb_coders = (int)tmp;
        else if (i == 1) data->time_to_burnout = (int)tmp;
        else if (i == 2) data->time_to_compile = (int)tmp;
        else if (i == 3) data->time_to_debug = (int)tmp;
        else if (i == 4) data->time_to_refactor = (int)tmp;
        else if (i == 5) data->number_of_compiles_required = (int)tmp;
        else if (i == 6) data->dongle_cooldown = (int)tmp;
        i++;
    }
    if (strcmp(av[7], "fifo") == 0 || strcmp(av[7], "edf") == 0)
        data->scheduler = av[7];
    else
    {
        fprintf(stderr, "Error, scheduler must be 'edf' or 'fifo'\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

int main(int ac, char **av)
{
    if (ac != 9)
    {
        fprintf(stderr, "Error, format of arguments must be:\n");
        fprintf(stderr, "number_of_coders time_to_burnout time_to_compile time_to_debug\n");
        fprintf(stderr, "time_to_refactor number_of_compiles_required dongle_cooldown scheduler");
        return (EXIT_FAILURE);
    }

    t_data data;
    init_data_struct(&data);
    if (fill_data_struct(&data, av + 1) == EXIT_FAILURE)
        return (EXIT_FAILURE);
    
    return (EXIT_SUCCESS);
}