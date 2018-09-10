/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decode_base64.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvignau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 18:52:07 by kvignau           #+#    #+#             */
/*   Updated: 2018/09/10 18:52:09 by kvignau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

char		*ft_check_invalid_char(char *str, int len, char *tmp, int i)
{
	int		j;

	j = 0;
	while (++i < len)
	{
		if (str[i] == '=')
		{
			tmp[j] = str[i];
			if (++i < len)
			{
				if (i % 4 == 2)
					return (ft_invalid_char());
				if (str[i] == '=')
					tmp[j + 1] = str[i];
				else if (ft_get_letter(str[i]) == -1 && (i % 4) != 0)
					return (ft_invalid_char());
			}
			break ;
		}
		if (str[i] != '\0' && ft_get_letter(str[i]) == -1)
			return (ft_invalid_char());
		tmp[j] = str[i];
		j++;
	}
	return (tmp);
}

char		*check_base64(char *str, t_opts opt)
{
	char	*tmp;
	int		i;
	int		j;

	i = -1;
	j = 0;
	tmp = (char *)malloc(sizeof(char) + opt.len);
	if (!tmp)
	{
		print_errors("Malloc error\n");
		return (NULL);
	}
	tmp = ft_bzero(tmp, opt.len);
	str = ft_memcpy(str, ft_cleanspace(str, tmp), opt.len);
	tmp = ft_bzero(tmp, opt.len);
	tmp = ft_check_invalid_char(str, opt.len, tmp, i);
	return (tmp);
}

void		ft_decode_mod4(char *str, int len, t_opts opt)
{
	int		i;
	int		j;
	int		save;

	i = 0;
	while (i < len)
	{
		j = -1;
		save = 0;
		while (++j < 4)
		{
			save = save | ft_get_letter(str[i + j]);
			if (j < 3)
				save = save << 6;
		}
		ft_putchar_fd(save >> 16, opt.fd);
		ft_putchar_fd(save >> 8, opt.fd);
		ft_putchar_fd(save, opt.fd);
		i += 4;
	}
}

void		ft_decode_reste(char *str, int len, int nb_equal, t_opts opt)
{
	int		save;

	save = 0;
	if (nb_equal > 0 && ((len + 1) % 4 == 0
		|| (len + nb_equal) % 4 == 0))
	{
		if (len % 4 == 2)
		{
			save = (ft_get_letter(str[len - 2]) << 2)
				| (ft_get_letter(str[len - 1]) >> 4);
			ft_putchar_fd(save, opt.fd);
		}
		else if (len % 4 == 3)
		{
			save = (ft_get_letter(str[len - 3]) << 10)
				| (ft_get_letter(str[len - 2]) << 4)
				| (ft_get_letter(str[len - 1]) >> 2);
			ft_putchar_fd(save >> 8, opt.fd);
			ft_putchar_fd(save, opt.fd);
		}
	}
}

int			ft_decode_base(char *str, t_opts opt)
{
	int		len;
	int		mod;
	int		nb_equal;

	nb_equal = 0;
	str = check_base64(str, opt);
	if (!str)
		return (EXIT_FAILURE);
	len = ft_strlen(str);
	while (len - nb_equal > 0 && str[(len - nb_equal) - 1] == '=')
		nb_equal++;
	len -= nb_equal;
	mod = len % 4;
	len -= mod;
	ft_decode_mod4(str, len, opt);
	len += mod;
	ft_decode_reste(str, len, nb_equal, opt);
	free(str);
	return (EXIT_SUCCESS);
}
