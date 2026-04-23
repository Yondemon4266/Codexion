/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:18 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/23 11:57:15 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"



int	init_all_data(t_data *data, int ac, char **av)
{
	if (ac != 9)
	{
		print_error_arguments();
		return (ERROR);
	}
	memset(data, 0, sizeof(t_data));
	if (pthread_mutex_init(&data->stop_lock, NULL) != 0)
		return (ERROR);
	if (pthread_mutex_init(&data->start_lock, NULL) != 0)
		return (ERROR);
	if (parse_data(data, av) == ERROR)
		return (ERROR);
	if (fill_dongles_coders(data) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
