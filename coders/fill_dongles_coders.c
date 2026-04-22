

#include "data.h"

static void	fill_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_coders)
	{
		data->dongles[i].released_time = 0;
		pthread_mutex_init(&(data->dongles[i].mutex), NULL);
		i++;
	}
}

static void	fill_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].times_compiled = 0;
		data->coders[i].left_dongle = NULL;
		data->coders[i].right_dongle = &data->dongles[i];
		data->coders[i].left_dongle = &data->dongles[(i + 1) % data->nb_coders];
		i++;
	}
}

int	fill_dongles_coders(t_data *data)
{
	if (init_dongles(data) == ERROR)
		return (ERROR);
	if (init_coders(data) == ERROR)
	{
        int i;

        i = 0;
        while (i < data->nb_coders)
        {
            pthread_mutex_destroy(&data->dongles[i].mutex);
            i++;
        }
		free(data->dongles);
		return (ERROR);
	}
	fill_dongles(data);
	fill_coders(data);
	return (SUCCESS);
}