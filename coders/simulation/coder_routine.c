/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:13:13 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/27 23:29:46 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"

static void	coders_wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->start_lock);
	while (coder->data->start_simulation == 0)
		pthread_cond_wait(&coder->data->start_cond, &coder->data->start_lock);
	pthread_mutex_unlock(&coder->data->start_lock);
}

void	*routine_coder(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	coders_wait_for_start(coder);
	if (coder->id % 2 == 0)
		usleep((coder->data->time_to_compile * 1000) / 2);
	while (check_simulation_status(coder->data) == 0)
	{
		pthread_mutex_lock(&coder->coder_lock);
		// while (can_i_compile(coder) == 0)
		// 	pthread_cond_wait(&coder->wait_compil_cond, &coder->coder_lock);
		pthread_mutex_unlock(&coder->coder_lock);
		if (check_simulation_status(coder->data) != 0)
			break ;
		usleep(coder->data->time_to_compile * 1000);
		increment_compilation(coder);
	}
	return (NULL);
}
