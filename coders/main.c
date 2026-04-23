/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:21 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/23 13:02:17 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

void	*routine_coder(void *arg)
{
	t_coder *coder = (t_coder *)arg;
	/* A IMPLEMENTER*/
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
		if (pthread_create(&data->coders[created_threads].coder, NULL, &routine_coder, data->coders) != 0)
		{
			/* A IMPLEMENTER*/
		}
		created_threads++;
	}
	
	while (i < created_threads)
	{
		if (pthread_join(data->coders[i].coder, NULL) != 0)
			continue ;
		i++;
	}
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
	cleanup_all(&data);
	
	if (pthread_create(&data.monitor, NULL, &routine_monitor, &data) != 0)
		return (ERROR);
	if (pthread_join(data.monitor, NULL) != 0)
		return (ERROR);
	

	return (EXIT_SUCCESS);
}