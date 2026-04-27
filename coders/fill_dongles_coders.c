/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_dongles_coders.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:14 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/27 09:46:19 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

static int	init_dongles(t_data *data)
{
	int	i;

	data->dongles = (t_dongle *)malloc(sizeof(t_dongle) * (data->nb_coders));
	if (!(data->dongles))
		return (ERROR);
	memset(data->dongles, 0, sizeof(t_dongle) * data->nb_coders);
	i = 0;
	while (i < data->nb_coders)
	{
		if (pthread_mutex_init(&(data->dongles[i].mutex), NULL) != 0)
		{
			free_dongles(data->dongles, i);
			return (ERROR);
		}
		data->dongles[i].queue[0] = NULL;
		data->dongles[i].queue[1] = NULL;
		i++;
	}
	data->init_flags.dongles_flag = 1;
	return (SUCCESS);
}

static int	init_coders(t_data *data)
{
	int	i;

	data->coders = (t_coder *)malloc(sizeof(t_coder) * (data->nb_coders));
	if (!(data->coders))
		return (ERROR);
	memset(data->coders, 0, sizeof(t_coder) * data->nb_coders);
	i = 0;
	while (i < data->nb_coders)
	{
		data->coders[i].data = data;
		data->coders[i].id = i + 1;
		data->coders[i].right_dongle = &data->dongles[i];
		data->coders[i].left_dongle = &data->dongles[(i + 1) % data->nb_coders];
		if (pthread_mutex_init(&data->coders[i].coder_lock, NULL) != 0)
		{
			free_coders_mutex(data, i);
			return (ERROR);
		}
		i++;
	}
	data->init_flags.coders_lock_flag = 1;
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