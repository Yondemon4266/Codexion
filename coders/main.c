/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:21 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/23 19:18:33 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

void	*routine_coder(void *arg)
{
	t_coder *coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->data->start_lock);
	while (coder->data->start_simulation == 0)
		pthread_cond_wait(&coder->data->start_cond, &coder->data->start_lock);
	pthread_mutex_unlock(&coder->data->start_lock);
	// pthread_mutex_lock(&coder->data->stop_lock);
	// while (coder->data->stop_simulation == 0)
	// {
			
	// }

	printf("Coder %d starts working...\n", coder->id);
    return (NULL);
}

void	*routine_monitor(void *arg)
{
	t_data *data;
	data = (t_data *) arg;
	
	int	i;
	int	created_threads;

	created_threads = 0;
	i = 0;
	while (created_threads < data->nb_coders)
	{
		if (pthread_create(&data->coders[created_threads].coder, NULL, &routine_coder, &data->coders[created_threads]) != 0)
		{
			pthread_mutex_lock(&data->stop_lock);
			data->stop_simulation = 1;
			pthread_mutex_unlock(&data->stop_lock);
			break ;
		}
		created_threads++;
	}
	if (i == created_threads)
	{
		pthread_mutex_lock(&data->start_lock);
		data->start_simulation = 1;
		pthread_cond_broadcast(&data->start_cond);
		pthread_mutex_unlock(&data->start_lock);
	}
	while (i < created_threads)
	{
		if (pthread_join(data->coders[i].coder, NULL) != 0)
			continue ;
		i++;
	}
	return (NULL);
}


int	main(int ac, char **av)
{
	t_data data;

	if (init_all_data(&data, ac, av + 1) == ERROR)
	{
		cleanup_all(&data);
		return (EXIT_FAILURE);
	}
	print_data_structure(&data);
	
	if (pthread_create(&data.monitor, NULL, &routine_monitor, &data) != 0)
		return (ERROR);
	if (pthread_join(data.monitor, NULL) != 0)
		return (ERROR);
	
	cleanup_all(&data);

	return (EXIT_SUCCESS);
}