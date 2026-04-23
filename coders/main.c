/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:21 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/23 12:13:42 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"



void	*routine_monitor(void *arg)
{
	t_data *data;
	data = (t_data *) arg;
	
	int	i;

	i = 0;
	while (i < data->nb_coders)
	{
		if (pthread_create(&data->coders[i], NULL, &coder_routine) != 0)
			
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