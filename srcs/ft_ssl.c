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

void		my_print_bits(char opt)
{
	int i;

	i = 7;
	ft_putstr("BITS -> ");
	while (i >= 0)
	{
		if ((opt >> i) & 0x1)
			ft_putnbr(1);
		else
			ft_putnbr(0);
		i--;	
	}
	ft_putchar('\n');
}

void    print_bits(unsigned char octet)
{
	int z = 128, oct = octet;

	while (z > 0)
	{
		if (oct & z)
			write(1, "1", 1);
		else
			write(1, "0", 1);
		z >>= 1;
	}
	ft_putchar('\n');
}

int		print_usage(char *str)
{
	ft_putstr("usage: ");
	ft_putstr(str);
	ft_putstr(" [hash] [command options] [command arguments]");
	return (-1);
}

void	ft_init_hash(t_hash *hash)
{
	(*hash).h[0] = 0x67452301;
	(*hash).h[1] = 0xEFCDAB89;
	(*hash).h[2] = 0x98BADCFE;
	(*hash).h[3] = 0x10325476;
	(*hash).g = 0;
	(*hash).funct = 0;
	(*hash).w = NULL;
	(*hash).h_mod[0] = 0;
	(*hash).h_mod[1] = 0;
	(*hash).h_mod[2] = 0;
	(*hash).h_mod[3] = 0;
	// (*hash).lo = 0;
	// (*hash).hi = 0;
	// return (hash);
}

void		print_md5(t_hash *hash)
{
	uint8_t *p;

	p = (uint8_t *)&hash->h[0];
	ft_printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
	p = (uint8_t *)&hash->h[1];
	ft_printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
	p = (uint8_t *)&hash->h[2];
	ft_printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
	p = (uint8_t *)&hash->h[3];
	ft_printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
}

void		ft_fill_hash(t_hash *hash, int choice)
{
	if (choice == 0)
	{
		hash->h_mod[0] = hash->h[0];
		hash->h_mod[1] = hash->h[1];
		hash->h_mod[2] = hash->h[2];
		hash->h_mod[3] = hash->h[3];
	}
	else
	{
		hash->h[0] += hash->h_mod[0];
		hash->h[1] += hash->h_mod[1];
		hash->h[2] += hash->h_mod[2];
		hash->h[3] += hash->h_mod[3];
	}
}

void		ft_rotate_md5(t_hash *hash, int i)
{
	int		tmp;

	tmp = hash->h_mod[3];
	hash->h_mod[3] = hash->h_mod[2];
	hash->h_mod[2] = hash->h_mod[1];
	hash->h_mod[1] = hash->h_mod[1] + LEFTROTATE((hash->h_mod[0] + hash->funct + g_k[i] + hash->w[hash->g]), g_r[i]);
	hash->h_mod[0] = tmp;
}

void		function_md5(t_hash *hash, int i)
{
	while (++i < 64)
	{
		if (i < 16)
		{
			hash->funct = F(hash->h_mod[1], hash->h_mod[2], hash->h_mod[3]);
			hash->g = i;
		}
		else if (i < 32)
		{
			hash->funct = G(hash->h_mod[1], hash->h_mod[2], hash->h_mod[3]);
			hash->g = (5 * i + 1) % 16;
		}
		else if (i < 48)
		{
			hash->funct = H(hash->h_mod[1], hash->h_mod[2], hash->h_mod[3]);
			hash->g = (3 * i + 5) % 16;
		}
		else
		{
			hash->funct = I(hash->h_mod[1], hash->h_mod[2], hash->h_mod[3]);
			hash->g = (7 * i) % 16;
		}
		ft_rotate_md5(hash, i);
	}
}

void		ft_hash_proc(t_md5 md5, t_hash *hash)
{
	size_t		offset;
	int		i;

	offset = 0;
	while (offset < md5.size_all)
	{
		hash->w = (uint32_t *)(md5.data + offset);
		i = -1;
		ft_fill_hash(hash, 0);
		function_md5(hash, i);
		ft_fill_hash(hash, 1);
		offset += 64;
	}
	print_md5(hash);
}

void		ft_md5_string(char *val)
{
	t_md5	md5;
	size_t 	msg_len_bits;
	t_hash	hash;

	ft_init_hash(&hash);
	md5.message_len = ft_strlen(val);
	md5.size_all = md5.message_len + 1;
	while (md5.size_all % 64 != 56)
		md5.size_all++;
	md5.size_all += 8;
	md5.data = (char *)malloc(sizeof(char) * md5.size_all);
	ft_memcpy(md5.data, val, md5.message_len);

	md5.data[md5.message_len] = (char)(1 << 7);
	ft_memset(md5.data + md5.message_len + 1, 0, md5.size_all - (md5.message_len + 1));
	msg_len_bits = md5.message_len * 8;
	ft_memcpy(md5.data + md5.size_all - 8, &msg_len_bits, 4);
	ft_hash_proc(md5, &hash);
}

void		ft_md5(char *val, char *opt)
{
// 	t_md5	md5;
// 	size_t 	msg_len_bits;
// 	t_hash	hash;

	if ((*opt) & OPT_S)
	{
		ft_md5_string(val);
		(*opt) = (*opt) & ~OPT_S;
		ft_putchar('\n');
		my_print_bits(*opt);
	}
	// GESTION FILE
	// ft_init_hash(&hash);
	// md5.message_len = ft_strlen(val);
	// md5.size_all = md5.message_len + 1;
	// while (md5.size_all % 64 != 56)
	// 	md5.size_all++;
	// md5.size_all += 8;
	// md5.data = (char *)malloc(sizeof(char) * md5.size_all);
	// ft_memcpy(md5.data, val, md5.message_len);

	// md5.data[md5.message_len] = (char)(1 << 7);
	// ft_memset(md5.data + md5.message_len + 1, 0, md5.size_all - (md5.message_len + 1));
	// msg_len_bits = md5.message_len * 8;
	// ft_memcpy(md5.data + md5.size_all - 8, &msg_len_bits, 4);
	// ft_hash_proc(md5, &hash);
}

static int			ft_check_options(char *arg, char *opt)
{
	int		i;

	i = 1;
	while (arg && arg[i])
	{
		if (arg[i] == 'p')
			(*opt) =	(*opt) | OPT_P;
		else if (arg[i] == 'q')
			(*opt) =	(*opt) | OPT_Q;
		else if (arg[i] == 'r')
			(*opt) =	(*opt) | OPT_R;
		else if (arg[i] == 's')
			(*opt) =	(*opt) | OPT_S;
		else
		{
			// ft_printf("ls: illegal option -- %c\n", arg[i]);
			// ft_printf("./ft_ls: illegal option -- %c\n", arg[i]);
			write(2, "ft_ssl: illegal option -- ", 22);
			write(2, &arg[i], 1);
			write(2, "\nusage: ls [-pqrs] [file ...]\n", 63);
			// write(2, "\nusage: ./ft_ls [larRt] [file ...]\n", 35);
			// ft_printf("usage: ./ft_ls [larRt] [file ...]\n");
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (1);
}

int			main(int argc, char **argv)
{
	int		i;
	char	opt;

	opt = 0;
	i = 2;
	if (argc < 2 || (ft_strcmp(argv[1], "md5") != 0 && ft_strcmp(argv[1], "sha256") != 0))
		return (print_usage(argv[0]));
	while (i < argc && argv[i][0] == '-')
	{
		if (argv[i][1] == '\0')
			break ;
		else if (argv[i][1] == '-')
		{
			i++;
			break ;
		}
		else if (ft_check_options(argv[i], &opt) == 0)
			return (EXIT_FAILURE);
		i++;
	}
	// TO DEL AFFICHER OPTION BIT A BIT !
	my_print_bits(opt);
	// AND TO DEL
	// AVANCER ARGV EN FONCTION DES ARGS ET DES OPTIONS
	ft_md5(argv[3], &opt);
	return (0);
}
