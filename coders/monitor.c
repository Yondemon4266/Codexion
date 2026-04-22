/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:25 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/22 23:23:34 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "data.h"


static int     run_coders(t_data *data)
{
    int	i;
    int created_count;

    created_count = 0;
	while (created_count < data->nb_coders)
	{
		if (pthread_create(&data->coders[created_count].coder, NULL, 
            routine, (void *) &data->coders[created_count]) != 0)
        {
            pthread_mutex_lock(&data->stop_lock);
            data->stop_simulation = 1;
            pthread_mutex_unlock(&data->stop_lock);
            break ;
        }
		created_count++;
	}
    i = 0;
	while (i < created_count)
	{
		if (pthread_join(data->coders[i].coder, NULL) != 0)
        {
            i++;
            continue ;
        }
		i++;
	}
    if (created_count < data->nb_coders)
        return (ERROR);
    return (SUCCESS);
}

int    run_monitor(t_data *data)
{
    if (pthread_create(&data->monitor, NULL, run_coders, (void *) &data) != 0)
		return (ERROR);
	if (pthread_join(data->monitor, NULL) != 0)
		return (ERROR);
    return (SUCCESS);
}