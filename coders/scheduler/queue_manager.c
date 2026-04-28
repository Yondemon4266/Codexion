/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 20:27:01 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/28 21:23:07 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"

static void	fifo_mode_subscribe(t_dongle *dongle, t_coder *coder)
{
	if (dongle->queue[0] == NULL)
		dongle->queue[0] = coder;
	else
		dongle->queue[1] = coder;
}

static void	edf_mode_subscribe(t_dongle *dongle, t_coder *coder)
{
	long long	my_deadline;
	long long	other_deadline;
	int			other_is_compiling;
	t_coder		*other;

	pthread_mutex_lock(&coder->coder_lock);
	my_deadline = coder->last_compile_start + coder->data->time_to_burnout;
	pthread_mutex_unlock(&coder->coder_lock);
	if (dongle->queue[0] == NULL)
		dongle->queue[0] = coder;
	else
	{
		other = dongle->queue[0];
		pthread_mutex_lock(&other->coder_lock);
		other_deadline = other->last_compile_start
			+ coder->data->time_to_burnout;
		other_is_compiling = other->is_compiling;
		pthread_mutex_unlock(&other->coder_lock);
		if (!other_is_compiling && (my_deadline < other_deadline 
			|| (my_deadline == other_deadline && coder->id < other->id)))
		{
			dongle->queue[1] = other;
			dongle->queue[0] = coder;
		}
		else
			dongle->queue[1] = coder;
	}
}

static void	subscribe_to_queue(t_dongle *dongle, t_coder *coder,
		enum e_scheduler mode)
{
	if (mode == MODE_FIFO)
		fifo_mode_subscribe(dongle, coder);
	else
		edf_mode_subscribe(dongle, coder);
}

void	request_dongles(t_dongle *left, t_dongle *right, t_coder *coder,
		enum e_scheduler mode)
{
	pthread_mutex_lock(&left->mutex);
	subscribe_to_queue(left, coder, mode);
	pthread_mutex_unlock(&left->mutex);
	pthread_mutex_lock(&right->mutex);
	subscribe_to_queue(right, coder, mode);
	pthread_mutex_unlock(&right->mutex);
}