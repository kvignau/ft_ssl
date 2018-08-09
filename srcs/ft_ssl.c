/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvignau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/09 10:37:08 by kvignau           #+#    #+#             */
/*   Updated: 2018/08/09 10:37:13 by kvignau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

int		print_usage(char *str)
{
	ft_putstr("usage: ");
	ft_putstr(str);
	ft_putstr(" [hash] [command options] [command arguments]");
	return (-1);
}

int			main(int argc, char **argv)
{
	// char	*message;
	// int h0 = 0x67452301;
	// int h1 = 0xEFCDAB89;
	// int h2 = 0x98BADCFE;
	// int h3 = 0x10325476;

	if (argc < 2)
		return (print_usage(argv[0]));

// 	if (ft_strlen(argv[1]) % 64 != 0)
// 	{
// 		message = (char *)malloc(sizeof(char) *  (ft_strlen(argv[1]) + (64 - ft_strlen(argv[1]) % 64)));
// #if (DEBUG == 1)
// 		printf("size malloced -> %zu\n", ft_strlen(argv[1]) + (64 - ft_strlen(argv[1]) % 64));
// #endif
// 		memcpy(message, argv[1], ft_strlen(argv[1]));
// 		message[ft_strlen(argv[1])] = (char)(1 << 7);
		// int i = ft_strlen(argv[1]) - 1;
// 		while (++i <  (ft_strlen(argv[1]) + (56 - ft_strlen(argv[1]) % 64)))
// 			message[i] = 0;
// 		int blocs = (ft_strlen(argv[1]) + (64 - ft_strlen(argv[1]) % 64)) / 64;
// #if (DEBUG == 1)
// 		printf("blocs -> %d\n",blocs);
// #endif
// 		ft_memcpy(message, argv[1], ft_strlen(argv[1]));
// 	}
	return (0);
}