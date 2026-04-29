/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 22:35:48 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/29 17:12:07 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"

long long	get_current_time_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	init_time_of_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_coders)
	{
		pthread_mutex_lock(&data->coders[i].coder_lock);
		data->coders[i].last_compile_start = data->simulation_start_time;
		pthread_mutex_unlock(&data->coders[i].coder_lock);
		i++;
	}
	return (SUCCESS);
}
