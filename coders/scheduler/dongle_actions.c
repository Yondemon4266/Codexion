/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 23:17:49 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/29 10:41:12 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"


int handle_cooldown(t_coder *coder)
{
	long long current_time;
	long long l_ready; 
	long long r_ready;
	long long wait_time = 0;
	
	r_ready = 0;
	pthread_mutex_lock(&coder->left_dongle->mutex);
	l_ready = coder->left_dongle->released_time + coder->data->dongle_cooldown;
	pthread_mutex_unlock(&coder->left_dongle->mutex);

	if (coder->left_dongle != coder->right_dongle)
	{
		pthread_mutex_lock(&coder->right_dongle->mutex);
		r_ready = coder->right_dongle->released_time + coder->data->dongle_cooldown;
		pthread_mutex_unlock(&coder->right_dongle->mutex);
	}

	current_time = get_current_time_ms();
	if (current_time == -1)
		return (ERROR);

	if (l_ready > current_time)
		wait_time = l_ready - current_time;
	if (r_ready > current_time && (r_ready - current_time) > wait_time)
		wait_time = r_ready - current_time;

	if (wait_time > 0)
		usleep(wait_time * 1000);
	return (SUCCESS);
}


static int	release_single_dongle(t_dongle *dongle)
{
	t_coder	*next_coder;
	long long released_time;

	pthread_mutex_lock(&dongle->mutex);
	dongle->released_time = get_current_time_ms();
	released_time = dongle->released_time;
	dongle->queue[0] = dongle->queue[1]; 
	dongle->queue[1] = NULL;             
	next_coder = dongle->queue[0];       
	pthread_mutex_unlock(&dongle->mutex);
	if (released_time == -1)
		return (ERROR);

	if (next_coder != NULL)
	{
		pthread_mutex_lock(&next_coder->wait_lock);
		pthread_cond_broadcast(&next_coder->wait_compil_cond);
		pthread_mutex_unlock(&next_coder->wait_lock);
	}
	return (SUCCESS);
}


int	release_dongles(t_coder *coder)
{
	if (release_single_dongle(coder->left_dongle) == ERROR)
		return (ERROR);
	if (coder->left_dongle != coder->right_dongle)
	{
		if (release_single_dongle(coder->right_dongle) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}