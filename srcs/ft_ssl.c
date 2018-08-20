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
	(*hash).h0 = 0x67452301;
	(*hash).h1 = 0xEFCDAB89;
	(*hash).h2 = 0x98BADCFE;
	(*hash).h3 = 0x10325476;
	(*hash).g = 0;
	(*hash).funct = 0;
	(*hash).w = NULL;
	(*hash).h0_modify = 0;
	(*hash).h1_modify = 0;
	(*hash).h2_modify = 0;
	(*hash).h3_modify = 0;
	// return (hash);
}

void		print_md5(t_hash *hash)
{
	uint8_t *p;

	p = (uint8_t *)&hash->h0;
	ft_printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
	p = (uint8_t *)&hash->h1;
	ft_printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
	p = (uint8_t *)&hash->h2;
	ft_printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
	p = (uint8_t *)&hash->h3;
	ft_printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
}

void		ft_fill_hash(t_hash *hash, int choice)
{
	if (choice == 0)
	{
		hash->h0_modify = hash->h0;
		hash->h1_modify = hash->h1;
		hash->h2_modify = hash->h2;
		hash->h3_modify = hash->h3;
	}
	else
	{
		hash->h0 += hash->h0_modify;
		hash->h1 += hash->h1_modify;
		hash->h2 += hash->h2_modify;
		hash->h3 += hash->h3_modify;
	}
}

void		ft_rotate_md5(t_hash *hash, int i)
{
	int		tmp;

	tmp = hash->h3_modify;
	hash->h3_modify = hash->h2_modify;
	hash->h2_modify = hash->h1_modify;
	hash->h1_modify = hash->h1_modify + LEFTROTATE((hash->h0_modify + hash->funct + g_k[i] + hash->w[hash->g]), g_r[i]);
	hash->h0_modify = tmp;
}

void		function_md5(t_hash *hash, int i)
{
	while (++i < 64)
	{
		if (i < 16)
		{
			hash->funct = F(hash->h1_modify, hash->h2_modify, hash->h3_modify);
			hash->g = i;
		}
		else if (i < 32)
		{
			hash->funct = G(hash->h1_modify, hash->h2_modify, hash->h3_modify);
			hash->g = (5 * i + 1) % 16;
		}
		else if (i < 48)
		{
			hash->funct = H(hash->h1_modify, hash->h2_modify, hash->h3_modify);
			hash->g = (3 * i + 5) % 16;
		}
		else
		{
			hash->funct = I(hash->h1_modify, hash->h2_modify, hash->h3_modify);
			hash->g = (7 * i) % 16;
		}
		ft_rotate_md5(hash, i);
	}
}

void		ft_hash(t_md5 md5, t_hash *hash)
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

void		ft_md5(char *val)
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
	ft_hash(md5, &hash);
}

int			main(int argc, char **argv)
{
	if (argc < 2)
		return (print_usage(argv[0]));
	ft_md5(argv[1]);
	return (0);
}