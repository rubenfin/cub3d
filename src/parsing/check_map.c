/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_map.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jade-haa <jade-haa@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/31 14:46:33 by jade-haa      #+#    #+#                 */
/*   Updated: 2024/06/07 17:35:49 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	check_if_valid(t_parsing *data, int **map_flood, int y, int x)
{
	if (map_flood[y][x] == 0)
	{
		if (data->map_width < x + 1 || map_flood[y][x + 1] == 2)
			return (0);
		if (data->map_height < y + 1 || map_flood[y + 1][x] == 2)
			return (0);
		if (y - 1 < 0 || map_flood[y - 1][x] == 2)
			return (0);
		if (x - 1 < 0 || map_flood[y][x - 1] == 2)
			return (0);
	}
	return (1);
}

int	check_map(t_parsing *data)
{
	int	y;
	int	x;

	y = 0;
	x = 0;
	while (y < data->map_height)
	{
		x = 0;
		while (x < data->map_width)
		{
			if (!check_if_valid(data, data->map_flood, y, x))
			{
				return (write(STDERR_FILENO,
						"Map not closed\n", 15), 0);
			}
			x++;
		}
		y++;
	}
	return (1);
}
