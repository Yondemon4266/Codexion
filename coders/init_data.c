
#include "data.h"

void	init_data(t_data *data)
{
	data->nb_coders = 0;
	data->time_to_burnout = 0;
	data->time_to_compile = 0;
	data->time_to_debug = 0;
	data->time_to_refactor = 0;
	data->number_of_compiles_required = 0;
	data->dongle_cooldown = 0;
	data->scheduler = NULL;
	data->coders = NULL;
	data->dongles = NULL;
}

int	init_dongles(t_data *data)
{
	int	i;

	data->dongles = (t_dongle *)malloc(sizeof(t_dongle) * (data->nb_coders));
	if (!(data->dongles))
		return (ERROR);
	i = 0;
	while (i < data->nb_coders)
	{
		data->dongles[i].released_time = 0;
		pthread_mutex_init(&(data->dongles[i].mutex), NULL);
		i++;
	}
	return (SUCCESS);
}

int	init_coders(t_data *data)
{
	data->coders = (t_coder *)malloc(sizeof(t_coder) * (data->nb_coders));
	if (!(data->coders))
		return (ERROR);

	int i;

	i = 0;
	while (i < data->nb_coders)
	{
		data->coders[i].id = 0;
		data->coders[i].times_compiled = 0;
		data->coders[i].left_dongle = NULL;
		data->coders[i].right_dongle = NULL;
		i++;
	}
	return (SUCCESS);
}