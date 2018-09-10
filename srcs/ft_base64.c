/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_base64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvignau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 12:59:45 by kvignau           #+#    #+#             */
/*   Updated: 2018/09/03 12:59:49 by kvignau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

char	g_base64[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
	't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', '+', '/'};

void	ft_print_base(int save, int size, t_opts opt)
{
	while (size >= 0)
	{
		ft_putchar_fd(g_base64[(save >> (6 * size)) & 0x3F], opt.fd);
		size--;
	}
}

char	*ft_invalid_char(void)
{
	print_errors("Invalid character in input stream.\n");
	return (NULL);
}

int		ft_get_letter(char c)
{
	int	i;

	i = -1;
	while (++i < 64)
	{
		if (c == g_base64[i])
			return (i);
	}
	return (-1);
}

char	*ft_cleanspace(char *str, char *tmp)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (str[++i])
	{
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			continue ;
		tmp[j] = str[i];
		j++;
	}
	return (tmp);
}

int		ft_base64_string(char *val, t_opts opt, char *name, int hash_choice)
{
	name = NULL;
	hash_choice = 0;
	if (opt.opt & OPT_E || !(opt.opt & OPT_D))
		return (ft_encode_base(val, opt));
	if (opt.opt & OPT_D)
		return (ft_decode_base(val, opt));
	return (EXIT_FAILURE);
}
