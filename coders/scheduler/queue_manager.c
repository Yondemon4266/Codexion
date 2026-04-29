#include "../data.h"

static void	fifo_mode_subscribe(t_dongle *dongle, t_coder *coder)
{
	if (dongle->queue[0] == NULL)
		dongle->queue[0] = coder;
	else
		dongle->queue[1] = coder;
}

static int	is_priority(long long my_dl, int my_tc, int my_id, t_coder *other)
{
	long long	o_dl;

	o_dl = other->last_compile_start + other->data->time_to_burnout;
	if (other->is_compiling)
		return (0);
	if (my_dl < o_dl)
		return (1);
	if (my_dl == o_dl && my_tc < other->times_compiled)
		return (1);
	if (my_dl == o_dl && my_tc == other->times_compiled)
	{
		if ((my_id % 2 != 0) && (other->id % 2 == 0))
			return (1);
		if ((my_id % 2 == 0) && (other->id % 2 != 0))
			return (0);
		if (my_id < other->id)
			return (1);
	}
	return (0);
}

static void	edf_mode_subscribe(t_dongle *dongle, t_coder *coder)
{
	long long	my_dl;
	int			my_tc;
	t_coder		*other;

	pthread_mutex_lock(&coder->coder_lock);
	my_dl = coder->last_compile_start + coder->data->time_to_burnout;
	my_tc = coder->times_compiled;
	pthread_mutex_unlock(&coder->coder_lock);
	if (dongle->queue[0] == NULL)
		dongle->queue[0] = coder;
	else
	{
		other = dongle->queue[0];
		pthread_mutex_lock(&other->coder_lock);
		if (is_priority(my_dl, my_tc, coder->id, other))
		{
			dongle->queue[1] = other;
			dongle->queue[0] = coder;
		}
		else
			dongle->queue[1] = coder;
		pthread_mutex_unlock(&other->coder_lock);
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