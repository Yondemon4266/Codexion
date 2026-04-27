/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:10:38 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/27 22:36:44 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"

void	stop_failed_simulation(t_data *data)
{
	pthread_mutex_lock(&data->stop_lock);
	data->stop_simulation = 1;
	data->simulation_state.sim_failed = 1;
	pthread_mutex_unlock(&data->stop_lock);
}

static void	create_threads(t_data *data)
{
	int	created_threads;

	created_threads = 0;
	while (created_threads < data->nb_coders)
	{
		if (pthread_create(&data->coders[created_threads].coder, NULL,
				&routine_coder, &data->coders[created_threads]) != 0)
		{
			data->simulation_state.count_created_threads = created_threads;
			fprintf(stderr, "Error, coder %d failed at creation",
				created_threads + 1);
			stop_failed_simulation(data);
			break ;
		}
		created_threads++;
	}
	data->simulation_state.count_created_threads = created_threads;
}

static int	start_simulation(t_data *data)
{
	create_threads(data);
	pthread_mutex_lock(&data->start_lock);
	data->start_simulation = 1;
	pthread_cond_broadcast(&data->start_cond);
	data->simulation_start_time = get_current_time_ms();
	pthread_mutex_unlock(&data->start_lock);
	if (data->simulation_start_time == -1)
		return (ERROR);
	if (data->simulation_state.count_created_threads != data->nb_coders
		|| data->simulation_state.sim_failed == 1)
		return (ERROR);
	return (SUCCESS);
}

static void	*routine_monitor(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	if (start_simulation(data) == ERROR)
		stop_failed_simulation(data);
	else
	{
		if (track_burnout(data) == ERROR)
			stop_failed_simulation(data);
		else
			stop_simulation(data);
	}
	i = 0;
	while (i < data->simulation_state.count_created_threads)
	{
		if (pthread_join(data->coders[i].coder, NULL) != 0)
			fprintf(stderr, "Error, coder %d couldn't be joined\n", i);
		i++;
	}
	return (data);
}

int	run_monitor(t_data *data)
{
	if (pthread_create(&data->monitor, NULL, &routine_monitor, data) != 0)
		return (ERROR);
	if (pthread_join(data->monitor, NULL) != 0)
		return (ERROR);
	if (data->simulation_state.sim_failed == 1)
		return (ERROR);
	return (SUCCESS);
}