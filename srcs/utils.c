/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvignau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/27 20:03:47 by kvignau           #+#    #+#             */
/*   Updated: 2018/08/27 20:03:50 by kvignau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

uint64_t	swap_uint64(uint64_t val)
{
	val = ((val << 8) & 0xFF00FF00FF00FF00ULL) |
	((val >> 8) & 0x00FF00FF00FF00FFULL);
	val = ((val << 16) & 0xFFFF0000FFFF0000ULL) |
	((val >> 16) & 0x0000FFFF0000FFFFULL);
	return (val << 32) | (val >> 32);
}

int			ft_open_file(int *fd, char *val)
{
	if (((*fd) = open(val, O_RDONLY)) < 0)
	{
		print_errors("ft_ssl: md5: ");
		print_errors(val);
		print_errors(": No such file or directory\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int			ft_files(char *val, char **str)
{
	int		fd;
	int		ret;
	char	buf[BUFF_SIZE + 1];

	if (ft_open_file(&fd, val) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while ((ret = read(fd, buf, BUFF_SIZE)) > 0)
	{
		if (ret < 0)
			return (print_errors("Read error"));
		buf[ret] = '\0';
		if (!(*str))
			(*str) = ft_strdup(buf);
		else
			(*str) = ft_strjoinandfree((*str), buf, 1);
	}
	close(fd);
	return (EXIT_SUCCESS);
}

int			print_usage(char *str)
{
	print_errors("usage: ");
	print_errors(str);
	print_errors(" [hash] [command options] [command arguments]\n");
	return (EXIT_FAILURE);
}

int			print_errors(char *str)
{
	write(2, str, ft_strlen(str));
	return (EXIT_FAILURE);
}
