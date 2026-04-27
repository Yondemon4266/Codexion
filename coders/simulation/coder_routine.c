/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:13:13 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/27 22:36:51 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"


// static void	print_coder(t_coder *coder, char *s)
// {
// 	long long	print_time;
	
// 	print_time = get_current_time_ms() - coder->data->simulation_start_time;
// 	if (print_time == -1)
// 		stop_failed_simulation(coder->data);
// 	else
// 	{
// 		pthread_mutex_lock(&coder->data->print_lock);
// 		printf("%lld %d %s\n", print_time, coder->id, s);
// 		pthread_mutex_unlock(&coder->data->print_lock);
// 	}
// }

static void	update_last_compile_start_time(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_lock);
	coder->last_compile_start = get_current_time_ms();
	if (coder->last_compile_start == -1)
		stop_failed_simulation(coder->data);
	pthread_mutex_unlock(&coder->coder_lock);
}

static void	increment_compilation(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_lock);
	coder->times_compiled++;
	pthread_mutex_unlock(&coder->coder_lock);
	update_last_compile_start_time(coder);
}

// static int can_i_compile(t_coder *coder)
// {
//     int can_compile = 0;
//     t_dongle *first = coder->left_dongle;
//     t_dongle *second = coder->right_dongle;

//     if (first > second)
//     {
//         first = coder->right_dongle;
//         second = coder->left_dongle;
//     }

//     pthread_mutex_lock(&first->mutex);
//     pthread_mutex_lock(&second->mutex);

//     if (coder->left_dongle->queue[0] == coder && coder->right_dongle->queue[0] == coder)
//         can_compile = 1;

//     pthread_mutex_unlock(&second->mutex);
//     pthread_mutex_unlock(&first->mutex);

//     return (can_compile);
// }

void	*routine_coder(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->data->start_lock);
	while (coder->data->start_simulation == 0)
		pthread_cond_wait(&coder->data->start_cond, &coder->data->start_lock);
	pthread_mutex_unlock(&coder->data->start_lock);
	
	if (coder->id % 2 == 0)
		usleep((coder->data->time_to_compile * 1000) / 2);
		
	while (check_simulation_status(coder->data) == 0)
	{
		pthread_mutex_lock(&coder->left_dongle->mutex);
		subscribe_to_queue(coder->left_dongle, coder, coder->data->scheduler);
		pthread_mutex_unlock(&coder->left_dongle->mutex);
		pthread_mutex_lock(&coder->right_dongle->mutex);
		subscribe_to_queue(coder->right_dongle, coder, coder->data->scheduler);
		pthread_mutex_unlock(&coder->right_dongle->mutex);

		pthread_mutex_lock(&coder->coder_lock);
		// while (can_i_compile(coder) == 0)
		// 	pthread_cond_wait(&coder->wait_compil_cond, &coder->coder_lock);
		pthread_mutex_unlock(&coder->coder_lock);
		if (check_simulation_status(coder->data) != 0)
			break ;
		
		usleep(coder->data->time_to_compile * 1000);
		increment_compilation(coder);
	}
	return (NULL);
}
