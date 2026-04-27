/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aluslu <aluslu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:25:27 by aluslu            #+#    #+#             */
/*   Updated: 2026/04/27 22:37:08 by aluslu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data.h"

static int	ft_custom_atoi(char *s, int *res_out)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	if (!s || !s[0])
		return (ERROR);
	while (s[i])
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result = (result * 10) + (s[i] - '0');
			if (result > INT_MAX)
				return (ERROR);
		}
		else
			return (ERROR);
		i++;
	}
	*res_out = (int)result;
	return (SUCCESS);
}

int	parse_data(t_data *data, char **av)
{
	if (ft_custom_atoi(av[0], &data->nb_coders) == ERROR
		|| ft_custom_atoi(av[1], &data->time_to_burnout) == ERROR
		|| ft_custom_atoi(av[2], &data->time_to_compile) == ERROR
		|| ft_custom_atoi(av[3], &data->time_to_debug) == ERROR
		|| ft_custom_atoi(av[4], &data->time_to_refactor) == ERROR
		|| ft_custom_atoi(av[5], &data->number_of_compiles_required) == ERROR
		|| ft_custom_atoi(av[6], &data->dongle_cooldown) == ERROR)
	{
		fprintf(stderr,
			"Error: Arguments must be positive integers (0 to INT_MAX)\n");
		return (ERROR);
	}
	if (strcmp(av[7], "fifo") == 0 || strcmp(av[7], "edf") == 0)
	{
		if (strcmp(av[7], "fifo") == 0)
			data->scheduler = MODE_FIFO;
		else
			data->scheduler = MODE_EDF;
	}
	else
	{
		fprintf(stderr, "Error: Scheduler must be 'edf' or 'fifo'\n");
		return (ERROR);
	}
	return (SUCCESS);
}