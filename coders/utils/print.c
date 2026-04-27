/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:34 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/27 22:36:27 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"

void	print_data_structure(t_data *data)
{
	printf("\n==========================================\n");
	printf("         🔍 DATA STRUCTURE DEBUG          \n");
	printf("==========================================\n");
	printf("🔢 nb_coders                   : %d\n", data->nb_coders);
	printf("🔥 time_to_burnout             : %d ms\n", data->time_to_burnout);
	printf("⚙️  time_to_compile             : %d ms\n", data->time_to_compile);
	printf("🐞 time_to_debug               : %d ms\n", data->time_to_debug);
	printf("🔨 time_to_refactor            : %d ms\n", data->time_to_refactor);
	printf("✅ compiles_required           : %d\n",
		data->number_of_compiles_required);
	printf("🧊 dongle_cooldown             : %d ms\n", data->dongle_cooldown);
	printf("==========================================\n\n");
}

void	print_error_arguments(void)
{
	fprintf(stderr, "Error: Wrong number of arguments\n");
	fprintf(stderr,
			("Usage: ./coders nb_coders t_burnout t_compile "
				"t_debug t_refactor nb_comp t_cooldown scheduler\n"));
}
