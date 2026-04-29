/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prefill_queue.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 13:49:13 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/29 17:09:48 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"

void	prefill_dongles_queue(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_coders)
	{
		data->dongles[i].queue[0] = &data->coders[i];
		data->dongles[i].queue[1] = &data->coders[(i + 1) % data->nb_coders];
	}
}
