/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:13:13 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/27 13:55:51 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"


static void	print_coder(t_coder *coder, char *s)
{
	long long	print_time;
	
	print_time = get_current_time_ms() - coder->data->simulation_start_time;
	if (print_time == -1)
		stop_failed_simulation(coder->data);
	else
	{
		pthread_mutex_lock(&coder->data->print_lock);
		printf("%lld %d %s\n", print_time, coder->id, s);
		pthread_mutex_unlock(&coder->data->print_lock);
	}
}

static void	update_last_compile_start_time(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_lock);
	coder->last_compile_start = get_current_time_ms();
	if (coder->last_compile_start == -1)
		stop_failed_simulation(coder->data);
	pthread_mutex_unlock(&coder->coder_lock);
}

static void	increment_compilation(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_lock);
	coder->times_compiled++;
	pthread_mutex_unlock(&coder->coder_lock);
	update_last_compile_start_time(coder);
}

void	*routine_coder(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->data->start_lock);
	while (coder->data->start_simulation == 0)
		pthread_cond_wait(&coder->data->start_cond, &coder->data->start_lock);
	pthread_mutex_unlock(&coder->data->start_lock);
	while (check_simulation_status(coder->data) == 0)
	{
		print_coder(coder, "has taken a dongle");
		increment_compilation(coder);
		usleep(1000);
	}
	return (NULL);
}
