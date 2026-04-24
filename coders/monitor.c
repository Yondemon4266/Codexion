/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:10:38 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/24 16:24:52 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"


static void	start_simulation(t_data *data)
{
	pthread_mutex_lock(&data->start_lock);
	data->start_simulation = 1;
	pthread_cond_broadcast(&data->start_cond);
	pthread_mutex_unlock(&data->start_lock);
}

static void	stop_simulation(t_data *data)
{
	pthread_mutex_lock(&data->stop_lock);
	data->stop_simulation = 1;
	data->simulation_state.sim_failed = 1;
	pthread_mutex_unlock(&data->stop_lock);
}

static int get_time(t_data *data)
{
    if (gettimeofday(&data->simulation_start_time, NULL) != 0)
		{
			fprintf(stderr, "Error, get time of day failed");
			stop_simulation(data);
            return (ERROR);
		}
    return (SUCCESS);
}

static void create_threads(t_data *data)
{
    int created_threads;

    created_threads = data->simulation_state.count_created_threads;
    while (created_threads < data->nb_coders)
	{
		if (pthread_create(&data->coders[created_threads].coder, NULL, 
            &routine_coder, &data->coders[created_threads]) != 0)
		{
			fprintf(stderr, "Error, pthread %d failed at creation", created_threads);
			stop_simulation(data);
			break ;
		}
		created_threads++;
	}
}

static void	*routine_monitor(void *arg)
{
	t_data *data;
	data = (t_data *) arg;
	
	int	i;
    
	start_simulation(data);
	if (data->simulation_state.count_created_threads == data->nb_coders 
        && data->simulation_state.sim_failed == 0)
            track_burnout(data);
    i = 0;
	while (i < data->simulation_state.count_created_threads)
	{
		if (pthread_join(data->coders[i].coder, NULL) != 0)
			fprintf(stderr, "Error, coder %d couldn't be joined\n", i);
		i++;
	}
	return (data);
}


int run_monitor(t_data *data)
{
	if (pthread_create(&data->monitor, NULL, &routine_monitor, &data) != 0)
		return (ERROR);
	if (pthread_join(data->monitor, NULL) != 0)
		return (ERROR);
    if (data->simulation_state.sim_failed == 1)
        return (ERROR);
    return (SUCCESS);
}