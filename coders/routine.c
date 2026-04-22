/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:31 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/22 22:23:23 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

void	*routine(void *arg)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	t_coder *coder = (t_coder *)arg;
    while (1)
    {
        pthread_mutex_lock(&coder->data->stop_lock);
        if (coder->data->stop_simulation)
        {
            pthread_mutex_unlock(&coder->data->stop_lock);
            break;
        }
        pthread_mutex_unlock(&coder->data->stop_lock);
    }
	return (NULL);
}