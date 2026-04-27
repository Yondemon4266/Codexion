
#include "../data.h"

void	print_coder(t_coder *coder, char *s)
{
	long long	print_time;

	print_time = get_current_time_ms() - coder->data->simulation_start_time;
	if (print_time == -1)
		stop_failed_simulation(coder->data);
	else
	{
		pthread_mutex_lock(&coder->data->print_lock);
		printf("%lld %d %s\n", print_time, coder->id, s);
		pthread_mutex_unlock(&coder->data->print_lock);
	}
}

int	can_i_compile(t_coder *coder)
{
	int			can_compile;
	t_dongle	*first;
	t_dongle	*second;

	can_compile = 0;
	first = coder->left_dongle;
	second = coder->right_dongle;
	if (first > second)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	if (coder->left_dongle->queue[0] == coder
		&& coder->right_dongle->queue[0] == coder)
		can_compile = 1;
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
	return (can_compile);
}

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
	update_last_compile_start_time(coder);
}
