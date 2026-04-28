
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


void compiling(t_coder *coder)
{
	update_last_compile_start_time(coder);
	print_coder(coder, "is compiling");
	usleep(coder->data->time_to_compile * 1000);
	increment_compilation(coder);
}

void debugging(t_coder *coder)
{
	print_coder(coder, "is debugging");
	usleep(coder->data->time_to_debug * 1000);
}

void refactoring(t_coder *coder)
{
	print_coder(coder, "is refactoring");
	usleep(coder->data->time_to_refactor * 1000);
}