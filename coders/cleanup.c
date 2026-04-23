/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:22:15 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/23 18:59:42 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

void	print_error_arguments(void)
{
	fprintf(stderr, "Error: Wrong number of arguments\n");
	fprintf(stderr,
			("Usage: ./coders nb_coders t_burnout t_compile "
				"t_debug t_refactor nb_comp t_cooldown scheduler\n"));
}



static void	free_dongles(t_dongle *dongles, int dongle_len)
{
	if (!dongles)
		return ;
	int i;

	i = 0;
	while (i < dongle_len)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		i++;
	}
	free(dongles);
}

void	cleanup_all(t_data *data)
{
	pthread_mutex_destroy(&data->stop_lock);
	pthread_mutex_destroy(&data->start_lock);
	pthread_cond_destroy(&data->start_cond);
	free_dongles(data->dongles, data->nb_coders);
	free(data->coders);
}

