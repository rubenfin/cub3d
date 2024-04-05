/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   read_file.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/29 13:53:36 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/04/04 18:29:52 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include "../../include/get_next_line.h"

int	remove_nl(char *str)
{
	int	len;

	len = strlen(str);
	while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == ' ' || str[len
			- 1] == '\t'))
	{
		len--;
	}
	return (len);
}

char	*skip_spaces(char *str)
{
	int	i;
	int	nl;

	i = 0;
	while (str[i] == ' ')
		i++;
	nl = remove_nl(str);
	return (ft_strndup(str + i, nl - i));
}

int	set_data(t_parsing *data, TEX_COLOR found, char *str)
{
	bool	already_exists;

	already_exists = false;
	if (found == NO)
	{
		if (!data->path_north_tex)
			data->path_north_tex = skip_spaces(str);
		else
			already_exists = true;
	}
	else if (found == SO)
	{
		if (!data->path_south_tex)
			data->path_south_tex = skip_spaces(str);
		else
			already_exists = true;
	}
	else if (found == WE)
	{
		if (!data->path_west_tex)
			data->path_west_tex = skip_spaces(str);
		else
			already_exists = true;
	}
	else if (found == EA)
	{
		if (!data->path_east_tex)
			data->path_east_tex = skip_spaces(str);
		else
			already_exists = true;
	}
	else if (found == F)
	{
		if (!data->floor_color)
			data->floor_color = skip_spaces(str);
		else
			already_exists = true;
	}
	else if (found == C)
	{
		if (!data->ceiling_color)
			data->ceiling_color = skip_spaces(str);
		else
			already_exists = true;
	}
	if (already_exists)
		return (write(STDERR_FILENO, "Duplicate textures/color\n", 25), 0);
	return (1);
}

int	check_if_tex_color(t_parsing *data, char *str)
{
	int	ret_value;

	ret_value = 1;
	str = skip_spaces(str);
	if (!ft_strncmp(str, "NO ", 3))
		ret_value = set_data(data, NO, str + 3);
	else if (!ft_strncmp(str, "SO ", 3))
		ret_value = set_data(data, SO, str + 3);
	else if (!ft_strncmp(str, "WE ", 3))
		ret_value = set_data(data, WE, str + 3);
	else if (!ft_strncmp(str, "EA ", 3))
		ret_value = set_data(data, EA, str + 3);
	else if (!ft_strncmp(str, "F ", 2))
		ret_value = set_data(data, F, str + 2);
	else if (!ft_strncmp(str, "C ", 2))
		ret_value = set_data(data, C, str + 2);
	return (ret_value);
}

int	check_if_tex_color_return(char *str)
{
	int	ret_value;

	ret_value = 1;
	str = skip_spaces(str);
	if (!ft_strncmp(str, "NO ", 3))
		ret_value = 0;
	else if (!ft_strncmp(str, "SO ", 3))
		ret_value = 0;
	else if (!ft_strncmp(str, "WE ", 3))
		ret_value = 0;
	else if (!ft_strncmp(str, "EA ", 3))
		ret_value = 0;
	else if (!ft_strncmp(str, "F ", 2))
		ret_value = 0;
	else if (!ft_strncmp(str, "C ", 2))
		ret_value = 0;
	return (ret_value);
}

int	xpm_file_check(char *str, int *fd)
{
	int		end;
	char	dotmpx[] = "mpx.";
	int		x;

	x = 0;
	end = ft_strlen(str) - 1;
	*fd = open(str, O_RDONLY);
	if (*fd == -1)
		return (write(STDERR_FILENO, "One of the textures is invalid\n", 31),
			0);
	while (str[end] == dotmpx[x])
	{
		x++;
		end--;
	}
	if (x != 4)
	{
		write(STDERR_FILENO, "One or more textures aren't .xpm files\n", 39);
		return (0);
	}
	else
		return (1);
}

int	check_tex_path(t_parsing *data, int ret)
{
	if (!ret)
		return (0);
	if (!xpm_file_check(data->path_north_tex, &data->fd_north_tex))
		return (0);
	if (!xpm_file_check(data->path_south_tex, &data->fd_south_tex))
		return (0);
	if (!xpm_file_check(data->path_east_tex, &data->fd_east_tex))
		return (0);
	if (!xpm_file_check(data->path_west_tex, &data->fd_west_tex))
		return (0);
	return (1);
}

int	malloc_color(char **color, char *str, int i)
{
	int	y;

	y = 0;
	while (str[i] && str[i] != ',')
	{
		if (y == 0 && str[i] == '0')
		{
			i++;
			continue ;
		}
		y++;
		i++;
	}
	*color = (char *)malloc(sizeof(char) * (y + 1));
	if (!*color)
		return (0);
	return (1);
}

int	color_valid_check(char *str, int ret)
{
	int		x;
	char	*color;
	int		i;
	int		y;

	y = 0;
	i = 0;
	x = 0;
	if (!ret)
		return (0);
	while (x < 3)
	{
		malloc_color(&color, str, i);
		y = 0;
		while (str[i] && str[i] != ',')
		{
			if (y == 0 && str[i] == '0')
			{
				i++;
				continue ;
			}
			color[y] = str[i];
			y++;
			i++;
		}
		color[y] = '\0';
		if (y > 4 || ft_atoi(color) > 255 || ft_atoi(color) < 0)
			return (write(STDERR_FILENO,
					"Color RGB range is between 0 and 255\n", 37), 0);
		ft_free(&color);
		i++;
		x++;
	}
	return (1);
}

int	color_missing_check(t_parsing *data, int ret)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	if (!ret)
		return (0);
	if (ft_isalnum(data->floor_color[i]))
		x++;
	while (data->floor_color[i])
	{
		if (data->floor_color[i] == ',' && ft_isalnum(data->floor_color[i + 1]))
			x++;
		i++;
	}
	if (x != 3)
		return (write(STDERR_FILENO, "Invalid color\n", 14), 0);
	i = 0;
	x = 0;
	if (ft_isalnum(data->ceiling_color[i]))
		x++;
	while (data->ceiling_color[i])
	{
		if (data->ceiling_color[i] == ',' && ft_isalnum(data->ceiling_color[i
				+ 1]))
			x++;
		i++;
	}
	if (x != 3)
		return (write(STDERR_FILENO, "Invalid color\n", 14), 0);
	return (1);
}

int	remove_whitespace(char **str, int ret)
{
	int		i;
	int		x;
	char	*tmp;

	i = 0;
	x = 0;
	if (!ret)
		return (ret);
	if (!ft_isdigit((*str)[i]) && (*str)[i] != ' ')
		return (write(STDERR_FILENO, "Non-digit symbol found in color\n", 32),
			0);
	while ((*str)[i])
	{
		if (!ft_isdigit((*str)[i]) && (*str)[i] != ',' && (*str)[i] != ' ')
			return (write(STDERR_FILENO, "Non-digit symbol found in color\n",
					32), 0);
		if ((*str)[i] != ' ')
			x++;
		i++;
	}
	tmp = (char *)malloc((x + 1) * sizeof(char));
	i = 0;
	x = 0;
	while ((*str)[i])
	{
		if ((*str)[i] != ' ')
		{
			tmp[x] = (*str)[i];
			x++;
		}
		i++;
	}
	tmp[x] = '\0';
	ft_free(str);
	*str = tmp;
	return (1);
}

int	find_rgb_part(char *str, char **part, int *i)
{
	int	x;

	x = *i;
	while (str[x] && str[x] != ',')
		x++;
	*part = (char *)malloc(sizeof(char) * (x + 1));
	x = 0;
	while (str[*i] && str[*i] != ',')
	{
		(*part)[x] = str[*i];
		(*i)++;
		x++;
	}
	if (str[*i] == ',')
		(*i)++;
	(*part)[x] = '\0';
	return (1);
}

int	set_hex_color(t_parsing *data, int ret)
{
	char	*r;
	char	*g;
	char	*b;
	int		i;

	i = 0;
	if (!ret)
		return (ret);
	while (data->floor_color[i])
	{
		find_rgb_part(data->floor_color, &r, &i);
		find_rgb_part(data->floor_color, &g, &i);
		find_rgb_part(data->floor_color, &b, &i);
	}
	printf("%s %s %s\n", r, g, b);
	return (1);
	// rgb_to_hex(r, g, b);
}

int	tex_color_filled(t_parsing *data)
{
	int	ret;

	ret = 1;
	if (!data->path_north_tex)
		ret = 0;
	if (!data->path_south_tex)
		ret = 0;
	if (!data->path_west_tex)
		ret = 0;
	if (!data->path_east_tex)
		ret = 0;
	if (!data->floor_color)
		ret = 0;
	if (!data->ceiling_color)
		ret = 0;
	if (ret == 0)
		write(STDERR_FILENO, "Textures or colors not filled\n", 30);
	if (ret == 1)
	{
		ret = remove_whitespace(&data->ceiling_color, ret);
		ret = remove_whitespace(&data->floor_color, ret);
		ret = color_missing_check(data, ret);
		ret = color_valid_check(data->ceiling_color, ret);
		ret = color_valid_check(data->floor_color, ret);
		ret = set_hex_color(data, ret);
		ret = check_tex_path(data, ret);
	}
	return (ret);
}

int	read_file(t_parsing *data)
{
	char	*gnl_output;

	gnl_output = NULL;
	data->map = ft_strdup("");
	while (1)
	{
		gnl_output = get_next_line(data->file_fd);
		if (!gnl_output)
			break ;
		if (!check_if_tex_color(data, gnl_output))
			return (ft_free(&gnl_output), 0);
		if (check_if_tex_color_return(gnl_output))
			data->map = ft_strjoin_gnl(data->map, gnl_output);
		ft_free(&gnl_output);
	}
	ft_free(&gnl_output);
	if (!tex_color_filled(data))
		return (0);
	print_tex_color(data);
	printf("data->map \n%s\n", data->map);
	return (1);
}
