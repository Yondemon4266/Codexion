/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_tracking.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 15:47:54 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/24 16:34:33 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

void    track_burnout(t_data *data)
{
    int i;

    i = 0;
    if (get_time(data) == SUCCESS)
    {
        while (check_simulation_status(data) == 0)
        {
            while (i < data->nb_coders)
            {
                
                i++;
            }
        }
    }
}