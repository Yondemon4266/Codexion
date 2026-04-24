/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:18 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/24 19:58:32 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

static int	init_mutexes_conds(t_data *data)
{
	if (pthread_mutex_init(&data->stop_lock, NULL) != 0)
		return (ERROR);
	data->init_flags.stop_init_flag = 1;
	if (pthread_mutex_init(&data->start_lock, NULL) != 0)
		return (ERROR);
	data->init_flags.start_init_flag = 1;
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		return (ERROR);
	data->init_flags.print_init_flag = 1;
	if (pthread_cond_init(&data->start_cond, NULL) != 0)
		return (ERROR);
	data->init_flags.start_cond_init_flag = 1;
	return (SUCCESS);
}

int	init_all_data(t_data *data, int ac, char **av)
{
	memset(data, 0, sizeof(t_data));
	if (ac != 9)
	{
		print_error_arguments();
		return (ERROR);
	}
	if (parse_data(data, av) == ERROR)
		return (ERROR);
	if (fill_dongles_coders(data) == ERROR)
		return (ERROR);
	if (init_mutexes_conds(data) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
