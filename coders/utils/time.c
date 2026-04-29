/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 22:35:48 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/29 18:05:52 by aluslu           ###   ########.fr       */
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

int	ft_usleep(long long time_in_ms, t_data *data)
{
	long long	start;
	long long	current;

	start = get_current_time_ms();
	if (start == -1)
		return (ERROR);
	while (1)
	{
		if (check_simulation_status(data) != 0)
			break ;
		current = get_current_time_ms();
		if (current == -1)
		{
			stop_failed_simulation(data);
			break ;
		}
		if ((current - start) >= time_in_ms)
			break ;
		usleep(500);
	}
	return (SUCCESS);
}
