/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:13:13 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/24 20:36:10 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"


void	print_coder(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->print_lock);
	printf("Coder %d starts working...\n", coder->id);
	pthread_mutex_unlock(&coder->data->print_lock);

}

void	*routine_coder(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->data->start_lock);
	while (coder->data->start_simulation == 0)
		pthread_cond_wait(&coder->data->start_cond, &coder->data->start_lock);
	pthread_mutex_unlock(&coder->data->start_lock);
	while (check_simulation_status(coder->data) == 0)
	{
		print_coder(coder);
		pthread_mutex_lock(&coder->coder_lock);
		coder->times_compiled++;
		pthread_mutex_unlock(&coder->coder_lock);
		usleep(1000);
	}
	return (NULL);
}
