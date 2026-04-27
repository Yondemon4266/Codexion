#include "../data.h"

static void	fifo_mode_subscribe(t_dongle *dongle, t_coder *coder)
{
	if (dongle->queue[0] == NULL)
		dongle->queue[0] = coder;
	else
		dongle->queue[1] = coder;
}

static void	edf_mode_subscribe(t_dongle *dongle, t_coder *coder)
{
	long long	my_deadline;
	long long	other_deadline;
	t_coder		*other;

	pthread_mutex_lock(&coder->coder_lock);
	my_deadline = coder->last_compile_start + coder->data->time_to_burnout;
	pthread_mutex_unlock(&coder->coder_lock);
	if (dongle->queue[0] == NULL)
		dongle->queue[0] = coder;
	else
	{
		other = dongle->queue[0];
		pthread_mutex_lock(&other->coder_lock);
		other_deadline = other->last_compile_start
			+ coder->data->time_to_burnout;
		pthread_mutex_unlock(&other->coder_lock);
		if (my_deadline < other_deadline || (my_deadline == other_deadline
				&& coder->id < other->id))
		{
			dongle->queue[1] = other;
			dongle->queue[0] = coder;
		}
		else
			dongle->queue[1] = coder;
	}
}

static void	subscribe_to_queue(t_dongle *dongle, t_coder *coder,
		enum e_scheduler mode)
{
	if (mode == MODE_FIFO)
		fifo_mode_subscribe(dongle, coder);
	else
		edf_mode_subscribe(dongle, coder);
}

void	request_dongles(t_dongle *left, t_dongle *right, t_coder *coder,
		enum e_scheduler mode)
{
	pthread_mutex_lock(&left->mutex);
	subscribe_to_queue(left, coder, mode);
	pthread_mutex_unlock(&left->mutex);
	if (left != right)
	{
		pthread_mutex_lock(&right->mutex);
		subscribe_to_queue(right, coder, mode);
		pthread_mutex_unlock(&right->mutex);
	}
}