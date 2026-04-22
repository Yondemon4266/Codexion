
#include "data.h"

int	main(int ac, char **av)
{
	t_data data;

	if (ac != 9)
	{
		fprintf(stderr, "Error: Wrong number of arguments\n");
		fprintf(stderr,
			"Usage: ./coders nb_coders t_burnout t_compile t_debug t_refactor nb_comp t_cooldown scheduler\n");
		return (EXIT_FAILURE);
	}
	init_data(&data);
	if (parse_data(&data, av + 1) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	printf("Simulation starting for %d coders with %s scheduler...\n",
		data.nb_coders, data.scheduler);
    fill_dongles_coders(&data);
	print_data_structure(&data);

	return (EXIT_SUCCESS);
}