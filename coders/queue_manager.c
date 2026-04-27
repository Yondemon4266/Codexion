/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 15:23:09 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/27 15:55:40 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "data.h"



void    subscribe_to_queue(t_dongle *dongle, t_coder *coder, enum e_scheduler mode)
{
    if (mode == MODE_FIFO)
    {
        if (dongle->queue[0] == NULL)
            dongle->queue[0] = coder;
        else
            dongle->queue[1] = coder;
    }
    else
    {
        long long my_deadline;

        pthread_mutex_lock(&coder->coder_lock);
        my_deadline = coder->last_compile_start + coder->data->time_to_burnout;
        pthread_mutex_unlock(&coder->coder_lock);

        if (dongle->queue[0] == NULL)
            dongle->queue[0] = coder;
        else
        {
            long long other_deadline;
            t_coder *other = dongle->queue[0];
            
            pthread_mutex_lock(&other->coder_lock);
            other_deadline = other->last_compile_start + coder->data->time_to_burnout;
            pthread_mutex_unlock(&other->coder_lock);
            if (my_deadline < other_deadline)
            {
                dongle->queue[1] = other;
                dongle->queue[0] = coder;
            }
            else
                dongle->queue[1] = coder;
        }
    }
}