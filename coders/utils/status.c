/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 22:36:18 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/29 17:05:39 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"

int	check_simulation_status(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->stop_lock);
	status = data->stop_simulation;
	pthread_mutex_unlock(&data->stop_lock);
	return (status);
}
