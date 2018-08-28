/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sha256.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvignau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/27 18:50:49 by kvignau           #+#    #+#             */
/*   Updated: 2018/08/27 18:50:54 by kvignau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

int32_t g_k_sh256[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static unsigned int		*get_tab(unsigned char *offset)
{
	int					i;
	unsigned int		*w;

	i = -1;
	w = (unsigned int *)malloc(sizeof(unsigned int) * 64);
	while (++i < 64)
	{
		if (i < 16)
			w[i] = (offset[i * 4] << 24) |
				(offset[i * 4 + 1] << 16) |
				(offset[i * 4 + 2] << 8) |
				(offset[i * 4 + 3]);
		else
			w[i] = D(w[i - 2]) + w[i - 7] + C(w[i - 15]) + w[i - 16];
	}
	return (w);
}

static void				function_sha256(t_hash *hash)
{
	int					i;

	i = -1;
	while (++i < 64)
	{
		(*hash).h_mod[8] = (*hash).h_mod[7] + B((*hash).h_mod[4]) +
			CH((*hash).h_mod[4], (*hash).h_mod[5], (*hash).h_mod[6])
			+ g_k_sh256[i] + (*hash).w[i];
		(*hash).h_mod[9] = A((*hash).h_mod[0]) + MAJ((*hash).h_mod[0],
			(*hash).h_mod[1], (*hash).h_mod[2]);
		(*hash).h_mod[7] = (*hash).h_mod[6];
		(*hash).h_mod[6] = (*hash).h_mod[5];
		(*hash).h_mod[5] = (*hash).h_mod[4];
		(*hash).h_mod[4] = (*hash).h_mod[3] + (*hash).h_mod[8];
		(*hash).h_mod[3] = (*hash).h_mod[2];
		(*hash).h_mod[2] = (*hash).h_mod[1];
		(*hash).h_mod[1] = (*hash).h_mod[0];
		(*hash).h_mod[0] = (*hash).h_mod[8] + (*hash).h_mod[9];
	}
}

static void				hash_proc_sha256(t_algo sha256, t_hash *hash, char opt)
{
	size_t				offset;
	int					i;

	offset = 0;
	while (offset < sha256.size_all - 8)
	{
		(*hash).w = get_tab((sha256.data + offset));
		i = -1;
		ft_fill_hash(hash, 0);
		function_sha256(hash);
		ft_fill_hash(hash, 1);
		offset += 64;
		free((*hash).w);
	}
	print_algo(hash, opt);
}

void					ft_print_hash_sha256(t_hash *hash)
{
	int					i;
	int					y;
	char				*res;

	i = -1;
	while (++i < 8)
	{
		res = ft_itoa_base_uimax(hash->h[i], 16);
		y = ft_strlen(res);
		while (y++ < 8)
			ft_putchar('0');
		ft_putstr(res);
		free(res);
	}
}

void					ft_sha256_string(char *val, char opt,
	char *name, int hash_choice)
{
	t_algo				sha256;
	size_t				msg_len_bits;
	t_hash				hash;

	ft_init_hash_sha256(&hash, name, hash_choice);
	ft_init_message(&sha256, val);
	sha256.data[sha256.message_len] = (char)(1 << 7);
	ft_memset(sha256.data + sha256.message_len + 1, 0,
		sha256.size_all - (sha256.message_len + 1));
	msg_len_bits = sha256.message_len * 8;
	msg_len_bits = swap_uint64(msg_len_bits);
	ft_memcpy(sha256.data + sha256.size_all - 8, &msg_len_bits, 8);
	hash_proc_sha256(sha256, &hash, opt);
	free(sha256.data);
}
