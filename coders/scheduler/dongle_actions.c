/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 23:17:49 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/28 13:33:26 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"


static void	release_single_dongle(t_dongle *dongle)
{
	t_coder	*next_coder;

	pthread_mutex_lock(&dongle->mutex);
	dongle->queue[0] = dongle->queue[1]; 
	dongle->queue[1] = NULL;             
	next_coder = dongle->queue[0];       
	pthread_mutex_unlock(&dongle->mutex);

	if (next_coder != NULL)
	{
		pthread_mutex_lock(&next_coder->coder_lock);
		pthread_cond_broadcast(&next_coder->wait_compil_cond);
		pthread_mutex_unlock(&next_coder->coder_lock);
	}
}

void	release_dongles(t_coder *coder)
{
	release_single_dongle(coder->left_dongle);
	if (coder->left_dongle != coder->right_dongle)
		release_single_dongle(coder->right_dongle);
}