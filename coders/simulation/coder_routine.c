/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:13:13 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/29 15:29:19 by aluslu           ###   ########.fr       */
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


static int	can_i_compile(t_coder *coder)
{
	int			can_compile;
	t_dongle	*first;
	t_dongle	*second;

	can_compile = 0;
	first = coder->left_dongle;
	second = coder->right_dongle;
	if (first > second)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	pthread_mutex_lock(&first->mutex);
	if (first != second)
		pthread_mutex_lock(&second->mutex);
	if (first != second && coder->left_dongle->queue[0] == coder
		&& coder->right_dongle->queue[0] == coder)
	{
		pthread_mutex_lock(&coder->coder_lock); 
		coder->is_compiling = 1;
		pthread_mutex_unlock(&coder->coder_lock);
		can_compile = 1;
	}
	if (first != second)
		pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
	return (can_compile);
}

static int wait_for_compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->wait_lock);
	while (can_i_compile(coder) == 0)
	{
		if (check_simulation_status(coder->data) != 0)
		{
			pthread_mutex_unlock(&coder->wait_lock);
			return (-1);
		}
		pthread_cond_wait(&coder->wait_compil_cond, &coder->wait_lock);
	}
	pthread_mutex_unlock(&coder->wait_lock);
	if (handle_cooldown(coder) == ERROR 
		|| check_simulation_status(coder->data) != 0)
		return (-1);
	print_coder(coder, "has taken a dongle");
	print_coder(coder, "has taken a dongle");
	return (0);
}


void	*routine_coder(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	coders_wait_for_start(coder);
	while (check_simulation_status(coder->data) == 0)
	{
		request_dongles(coder->left_dongle, coder->right_dongle,
			 coder, coder->data->scheduler);
		if (wait_for_compile(coder) == -1)
			break ;
		compiling(coder);
		pthread_mutex_lock(&coder->coder_lock);
		coder->is_compiling = 0;
		pthread_mutex_unlock(&coder->coder_lock);
		if (release_dongles(coder) == ERROR)
		{
			stop_failed_simulation(coder->data);
			break ;
		}
		debugging(coder);
		refactoring(coder);
	}
	return (NULL);
}
