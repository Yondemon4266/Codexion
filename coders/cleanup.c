/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:22:15 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/24 19:41:46 by aluslu           ###   ########.fr       */
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

void	free_coders_mutex(t_data *data, int offset)
{
	int	i;

	i = 0;
	while (i < offset)
	{
		pthread_mutex_destroy(&data->coders[i].coder_lock);
		i++;
	}
}

static void	free_dongles(t_dongle *dongles, int dongle_len)
{
	int	i;

	if (!dongles)
		return ;
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
	if (data->init_flags.stop_init_flag == 1)
		pthread_mutex_destroy(&data->stop_lock);
	if (data->init_flags.start_init_flag == 1)
		pthread_mutex_destroy(&data->start_lock);
	if (data->init_flags.start_cond_init_flag == 1)
		pthread_cond_destroy(&data->start_cond);
	if (data->init_flags.print_init_flag == 1)
		pthread_mutex_destroy(&data->print_lock);
	if (data->init_flags.coders_lock_flag == 1)
		free_coders_mutex(data, data->nb_coders);
	free_dongles(data->dongles, data->nb_coders);
	free(data->coders);
}
