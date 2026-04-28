
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
	update_last_compile_start_time(coder);
}


void compiling(t_coder *coder)
{
	usleep(coder->data->time_to_compile * 1000);
	print_coder(coder, "is compiling");
	increment_compilation(coder);
}

void debugging(t_coder *coder)
{
	usleep(coder->data->time_to_debug * 1000);
	print_coder(coder, "is debugging");
}

void refactoring(t_coder *coder)
{
	usleep(coder->data->time_to_refactor * 1000);
	print_coder(coder, "is refactoring");
}