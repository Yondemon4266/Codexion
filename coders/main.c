/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:21 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/27 22:06:33 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

int	main(int ac, char **av)
{
	t_data data;

	if (init_all_data(&data, ac, av + 1) == ERROR)
	{
		cleanup_all(&data);
		return (EXIT_FAILURE);
	}
	print_data_structure(&data);

	if (run_monitor(&data) == ERROR)
	{
		cleanup_all(&data);
		return (EXIT_FAILURE);
	}
	printf("burn out state: %d\n", data.simulation_state.burned_out);
	printf("success: %d\n", data.simulation_state.sim_success);

	cleanup_all(&data);

	return (EXIT_SUCCESS);
}