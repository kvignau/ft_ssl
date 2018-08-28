/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_md5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvignau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/27 19:16:14 by kvignau           #+#    #+#             */
/*   Updated: 2018/08/27 19:16:16 by kvignau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

int32_t g_r[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17,
	22, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 4, 11, 16, 23,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10, 15, 21, 6, 10, 15, 21,
	6, 10, 15, 21, 6, 10, 15, 21};

int32_t g_k_md5[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

void		ft_print_hash_md5(t_hash *hash)
{
	int		i;
	int		j;
	uint8_t *p;
	char	*res;

	i = 0;
	while (i < 4)
	{
		p = (uint8_t *)&hash->h[i];
		j = 0;
		while (j < 4)
		{
			res = ft_itoa_base_uimax(p[j], 16);
			if (ft_strlen(res) == 1)
				ft_putchar('0');
			ft_putstr(res);
			free(res);
			res = NULL;
			j++;
		}
		i++;
	}
}

static void	ft_rotate_md5(t_hash *hash, int i)
{
	int		tmp;

	tmp = hash->h_mod[3];
	hash->h_mod[3] = hash->h_mod[2];
	hash->h_mod[2] = hash->h_mod[1];
	hash->h_mod[1] = hash->h_mod[1] + LEFTROTATE((hash->h_mod[0]
		+ hash->funct + g_k_md5[i] + hash->w[hash->g]), g_r[i]);
	hash->h_mod[0] = tmp;
}

static void	function_md5(t_hash *hash)
{
	int		i;

	i = -1;
	while (++i < 64)
	{
		if (i < 16)
			(hash->funct = F(hash->h_mod[1], hash->h_mod[2], hash->h_mod[3])) ?
				(hash->g = i) : 0;
		else if (i < 32)
			(hash->funct = G(hash->h_mod[1], hash->h_mod[2], hash->h_mod[3])) ?
				(hash->g = (5 * i + 1) % 16) : 0;
		else if (i < 48)
			(hash->funct = H(hash->h_mod[1], hash->h_mod[2], hash->h_mod[3])) ?
				(hash->g = (3 * i + 5) % 16) : 0;
		else
			(hash->funct = I(hash->h_mod[1], hash->h_mod[2], hash->h_mod[3])) ?
				(hash->g = (7 * i) % 16) : 0;
		ft_rotate_md5(hash, i);
	}
}

static void	ft_hash_proc(t_algo md5, t_hash *hash, char opt)
{
	size_t	offset;

	offset = 0;
	while (offset < md5.size_all)
	{
		hash->w = (uint32_t *)(md5.data + offset);
		ft_fill_hash(hash, 0);
		function_md5(hash);
		ft_fill_hash(hash, 1);
		offset += 64;
	}
	print_algo(hash, opt);
}

void		ft_md5_string(char *val, char opt, char *name, int hash_choice)
{
	t_algo	md5;
	size_t	msg_len_bits;
	t_hash	hash;

	ft_init_hash_md5(&hash, name, hash_choice);
	ft_init_message(&md5, val);
	md5.data[md5.message_len] = (char)(1 << 7);
	ft_memset(md5.data + md5.message_len
		+ 1, 0, md5.size_all - (md5.message_len + 1));
	msg_len_bits = md5.message_len * 8;
	ft_memcpy(md5.data + md5.size_all - 8, &msg_len_bits, 4);
	ft_hash_proc(md5, &hash, opt);
	free(md5.data);
}
