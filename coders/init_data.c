/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:18 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/22 16:25:19 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

static void	init_data(t_data *data)
{
	data->nb_coders = 0;
	data->time_to_burnout = 0;
	data->time_to_compile = 0;
	data->time_to_debug = 0;
	data->time_to_refactor = 0;
	data->number_of_compiles_required = 0;
	data->dongle_cooldown = 0;
	data->scheduler = NULL;
	data->coders = NULL;
	data->dongles = NULL;
}

int	init_all_data(t_data *data, int ac, char **av)
{
	if (ac != 9)
	{
		print_error_arguments();
		return (ERROR);
	}
	init_data(data);
	if (parse_data(data, av) == ERROR)
		return (ERROR);
	if (fill_dongles_coders(data) == ERROR)
	{
		free_dongles(data->dongles, data->nb_coders);
		return (ERROR);
	}
	return (SUCCESS);
}
