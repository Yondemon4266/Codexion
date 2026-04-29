/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 17:10:24 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/29 17:56:47 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"

void	update_last_compile_start_time(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_lock);
	coder->last_compile_start = get_current_time_ms();
	if (coder->last_compile_start == -1)
		stop_failed_simulation(coder->data);
	pthread_mutex_unlock(&coder->coder_lock);
}

void	increment_compilation(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_lock);
	coder->times_compiled++;
	pthread_mutex_unlock(&coder->coder_lock);
}

void	compiling(t_coder *coder)
{
	update_last_compile_start_time(coder);
	print_coder(coder, "is compiling");
	ft_usleep(coder->data->time_to_compile, coder->data);
	increment_compilation(coder);
	pthread_mutex_lock(&coder->coder_lock);
	coder->is_compiling = 0;
	pthread_mutex_unlock(&coder->coder_lock);
}

void	debugging(t_coder *coder)
{
	print_coder(coder, "is debugging");
	ft_usleep(coder->data->time_to_debug, coder->data);
}

void	refactoring(t_coder *coder)
{
	print_coder(coder, "is refactoring");
	ft_usleep(coder->data->time_to_refactor, coder->data);
}
