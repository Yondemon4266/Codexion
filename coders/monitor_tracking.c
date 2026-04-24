/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_tracking.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 15:47:54 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/24 19:12:10 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

static void	stop_simulation(t_data *data)
{
	pthread_mutex_lock(&data->stop_lock);
	data->stop_simulation = 1;
	pthread_mutex_unlock(&data->stop_lock);
}

static int	coder_check_times_compiled(t_coder *coder)
{
	int	status;

	status = 1;
	pthread_mutex_lock(&coder->coder_lock);
	if (coder->times_compiled < coder->data->number_of_compiles_required)
		status = 0;
	pthread_mutex_unlock(&coder->coder_lock);
	return (status);
}

static int	is_coder_burnt_out(t_coder *coder)
{
	long long	current_time;
	long long	last_compile;
	long long	time_elapsed;

	current_time = get_current_time_ms();
	if (current_time == -1)
		return (ERROR);
	pthread_mutex_lock(&coder->coder_lock);
	last_compile = coder->last_compile_start;
	pthread_mutex_unlock(&coder->coder_lock);
	time_elapsed = current_time - last_compile;
	if (time_elapsed >= coder->data->time_to_burnout)
		return (2);
	return (0);
}

static int	check_all_coders(t_data *data)
{
	int	i;
	int	finished;
	int	status;

	i = -1;
	finished = 0;
	while (++i < data->nb_coders)
	{
		status = is_coder_burnt_out(&data->coders[i]);
		if (status == ERROR)
			return (ERROR);
		if (status == 2)
		{
			stop_simulation(data);
			return (1);
		}
		if (coder_check_times_compiled(&data->coders[i]) == 1)
			finished++;
	}
	if (finished == data->nb_coders)
		stop_simulation(data);
	return (finished == data->nb_coders);
}


int	track_burnout(t_data *data)
{
	int	status;

	while (check_simulation_status(data) == 0)
	{
		status = check_all_coders(data);
		if (status == ERROR)
			return (ERROR);
		if (status == 1)
			break ;
		usleep(1000);
	}
	return (SUCCESS);
}