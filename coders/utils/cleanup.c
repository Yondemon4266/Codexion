/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:22:15 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/29 11:30:39 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"

void	free_coders_cond(t_data *data, int offset)
{
	int	i;

	i = 0;
	while (i < offset)
	{
		pthread_cond_destroy(&data->coders[i].wait_compil_cond);
		i++;
	}
}

void	free_coders_mutex(t_data *data, int offset)
{
	int	i;

	i = 0;
	while (i < offset)
	{
		pthread_mutex_destroy(&data->coders[i].coder_lock);
		pthread_mutex_destroy(&data->coders[i].wait_lock);
		i++;
	}
}

void	free_dongles_mutex(t_dongle *dongles, int offset)
{
	int	i;

	if (!dongles)
		return ;
	i = 0;
	while (i < offset)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		i++;
	}
}

static void	cleanup_global_mutexes(t_data *data)
{
	if (data->init_flags.stop_init_flag == 1)
		pthread_mutex_destroy(&data->stop_lock);
	if (data->init_flags.start_init_flag == 1)
		pthread_mutex_destroy(&data->start_lock);
	if (data->init_flags.start_cond_init_flag == 1)
		pthread_cond_destroy(&data->start_cond);
	if (data->init_flags.print_init_flag == 1)
		pthread_mutex_destroy(&data->print_lock);
}

void	cleanup_all(t_data *data)
{
	cleanup_global_mutexes(data);
	if (data->init_flags.coders_lock_flag == 1)
		free_coders_mutex(data, data->nb_coders);
	if (data->init_flags.dongles_flag == 1)
		free_dongles_mutex(data->dongles, data->nb_coders);
	if (data->init_flags.coders_cond_flag == 1)
		free_coders_cond(data, data->nb_coders);
	if (data->coders)
		free(data->coders);
	if (data->dongles)
		free(data->dongles);
}