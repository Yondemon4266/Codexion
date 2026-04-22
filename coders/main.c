/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:21 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/22 22:28:35 by aluslu           ###   ########.fr       */
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
	cleanup_all(&data);
	

	return (EXIT_SUCCESS);
}