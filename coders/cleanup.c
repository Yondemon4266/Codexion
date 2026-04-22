/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:22:15 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/22 16:11:42 by aluslu           ###   ########.fr       */
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

void	free_dongles(t_dongle *dongles, int dongle_len)
{
	int i;

	i = 0;
	while (i < dongle_len)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		i++;
	}
	free(dongles);
}