/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_dongles_coders.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 23:21:33 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/29 17:06:20 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"

static int	init_dongles(t_data *data)
{
	int	i;

	data->dongles = (t_dongle *)malloc(sizeof(t_dongle) * (data->nb_coders));
	if (!(data->dongles))
		return (ERROR);
	memset(data->dongles, 0, sizeof(t_dongle) * data->nb_coders);
	i = -1;
	while (++i < data->nb_coders)
	{
		if (pthread_mutex_init(&(data->dongles[i].mutex), NULL) != 0)
		{
			free_dongles_mutex(data->dongles, i);
			return (ERROR);
		}
		data->dongles[i].queue[0] = NULL;
		data->dongles[i].queue[1] = NULL;
	}
	data->init_flags.dongles_flag = 1;
	return (SUCCESS);
}

static int	init_coder_primitives(t_coder *coder)
{
	if (pthread_mutex_init(&coder->coder_lock, NULL) != 0)
		return (ERROR);
	if (pthread_mutex_init(&coder->wait_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&coder->coder_lock);
		return (ERROR);
	}
	if (pthread_cond_init(&coder->wait_compil_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&coder->wait_lock);
		pthread_mutex_destroy(&coder->coder_lock);
		return (ERROR);
	}
	return (SUCCESS);
}

static int	init_coders_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_coders)
	{
		data->coders[i].data = data;
		data->coders[i].id = i + 1;
		data->coders[i].right_dongle = &data->dongles[i];
		data->coders[i].left_dongle = &data->dongles[(i + 1) % data->nb_coders];
		if (init_coder_primitives(&data->coders[i]) == ERROR)
		{
			free_coders_mutex(data, i);
			free_coders_cond(data, i);
			return (ERROR);
		}
	}
	return (SUCCESS);
}

static int	init_coders(t_data *data)
{
	data->coders = (t_coder *)malloc(sizeof(t_coder) * (data->nb_coders));
	if (!(data->coders))
		return (ERROR);
	memset(data->coders, 0, sizeof(t_coder) * data->nb_coders);
	if (init_coders_data(data) == ERROR)
		return (ERROR);
	data->init_flags.coders_lock_flag = 1;
	data->init_flags.coders_cond_flag = 1;
	return (SUCCESS);
}

int	fill_dongles_coders(t_data *data)
{
	if (init_dongles(data) == ERROR)
		return (ERROR);
	if (init_coders(data) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
