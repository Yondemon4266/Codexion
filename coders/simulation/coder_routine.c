/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:13:13 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/28 15:21:02 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"


void	print_coder(t_coder *coder, char *s)
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
	if (coder->left_dongle->queue[0] == coder
		&& coder->right_dongle->queue[0] == coder)
		can_compile = 1;
	if (first != second)
		pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
	return (can_compile);
}

void	*routine_coder(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	coders_wait_for_start(coder);
	if (coder->id % 2 == 0)
		usleep(2000);
	while (check_simulation_status(coder->data) == 0)
	{
		request_dongles(coder->left_dongle, coder->right_dongle,
			 coder, coder->data->scheduler);
		pthread_mutex_lock(&coder->coder_lock);
		while (can_i_compile(coder) == 0)
			pthread_cond_wait(&coder->wait_compil_cond, &coder->coder_lock);
		pthread_mutex_unlock(&coder->coder_lock);
		if (check_simulation_status(coder->data) != 0)
			break ;
		compiling(coder);
		release_dongles(coder);
		debugging(coder);
		refactoring(coder);
	}
	return (NULL);
}
