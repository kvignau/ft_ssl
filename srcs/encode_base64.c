/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encode_base64.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvignau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 18:51:59 by kvignau           #+#    #+#             */
/*   Updated: 2018/09/10 18:52:01 by kvignau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

void	ft_encode_last_two(char *str, int len, t_opts opt)
{
	int	save;

	save = 0;
	save = (str[((len) - 1)] << 8 & 0x00ff00) | (str[(len)] & 0x0000ff);
	save = save << 2;
	ft_print_base(save, 2, opt);
	ft_putchar_fd('=', opt.fd);
}

void	ft_encode_last(char *str, int len, t_opts opt)
{
	int	save;

	save = 0;
	save = str[len] & 0x0000ff;
	save = save << 4;
	ft_print_base(save, 1, opt);
	ft_putstr_fd("==", opt.fd);
}

int		ft_encode_base(char *str, t_opts opt)
{
	int	mod;
	int save;
	int i;
	int	len;

	i = 0;
	mod = (opt.len % 3);
	len = opt.len - mod;
	while (i < len)
	{
		save = 0;
		save = (str[i] << 16 & 0xff0000) | (str[i + 1] << 8 & 0x00ff00)
			| (str[i + 2] & 0x0000ff);
		ft_print_base(save, 3, opt);
		i += 3;
	}
	if (mod == 1)
		ft_encode_last(str, len + (mod - 1), opt);
	else if (mod == 2)
		ft_encode_last_two(str, len + (mod - 1), opt);
	len ? ft_putchar_fd('\n', opt.fd) : 0;
	return (EXIT_SUCCESS);
}
