/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:34 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/27 16:46:37 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

void	print_data_structure(t_data *data)
{
	printf("\n==========================================\n");
	printf("         🔍 DATA STRUCTURE DEBUG          \n");
	printf("==========================================\n");
	printf("🔢 nb_coders                   : %d\n", data->nb_coders);
	printf("🔥 time_to_burnout             : %d ms\n", data->time_to_burnout);
	printf("⚙️  time_to_compile             : %d ms\n", data->time_to_compile);
	printf("🐞 time_to_debug               : %d ms\n", data->time_to_debug);
	printf("🔨 time_to_refactor            : %d ms\n", data->time_to_refactor);
	printf("✅ compiles_required           : %d\n",
		data->number_of_compiles_required);
	printf("🧊 dongle_cooldown             : %d ms\n", data->dongle_cooldown);
	printf("==========================================\n\n");
}


int	check_simulation_status(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->stop_lock);
	status = data->stop_simulation;
	pthread_mutex_unlock(&data->stop_lock);
	return (status);
}

long long	get_current_time_ms(void)
{
	struct timeval time;

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