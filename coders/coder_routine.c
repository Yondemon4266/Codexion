/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:13:13 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/24 16:29:45 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "data.h"



int coder_check_times_compiled(t_coder *coder)
{
    int status;

    status = 1;
    pthread_mutex_lock(&coder->coder_lock);
    if (coder->times_compiled < coder->data->number_of_compiles_required)
        status = 0;
    pthread_mutex_unlock(&coder->coder_lock);
    return status;
}

void	*routine_coder(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    pthread_mutex_lock(&coder->data->start_lock);
    while (coder->data->start_simulation == 0)
        pthread_cond_wait(&coder->data->start_cond, &coder->data->start_lock);
    pthread_mutex_unlock(&coder->data->start_lock);
    
    while (check_simulation_status(coder->data) == 0)
    {
            printf("time: %ld", coder->data->simulation_start_time.tv_sec);
            break ;
    }

    printf("Coder %d starts working...\n", coder->id);
    return (NULL);
}
