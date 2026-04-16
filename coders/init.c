
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