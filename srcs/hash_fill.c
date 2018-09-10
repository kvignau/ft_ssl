/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_fill.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvignau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/27 20:01:27 by kvignau           #+#    #+#             */
/*   Updated: 2018/08/27 20:01:29 by kvignau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

void		ft_init_message(t_algo *md5, char *val, t_opts opt)
{
	md5->message_len = opt.len;
	md5->size_all = md5->message_len + 1;
	while (md5->size_all % 64 != 56)
		md5->size_all++;
	md5->size_all += 8;
	md5->data = (unsigned char *)malloc(sizeof(unsigned char) * md5->size_all);
	ft_memcpy(md5->data, val, md5->message_len);
}

void		ft_init_hash_md5(t_hash *hash, char *name, int hash_choice)
{
	(*hash).file_name = name;
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
	(*hash).hash_choice = hash_choice;
}

void		ft_init_hash_sha256(t_hash *hash, char *name, int hash_choice)
{
	(*hash).file_name = name;
	(*hash).h[0] = 0x6a09e667;
	(*hash).h[1] = 0xbb67ae85;
	(*hash).h[2] = 0x3c6ef372;
	(*hash).h[3] = 0xa54ff53a;
	(*hash).h[4] = 0x510e527f;
	(*hash).h[5] = 0x9b05688c;
	(*hash).h[6] = 0x1f83d9ab;
	(*hash).h[7] = 0x5be0cd19;
	(*hash).g = 0;
	(*hash).funct = 0;
	(*hash).w = NULL;
	(*hash).h_mod[0] = 0;
	(*hash).h_mod[1] = 0;
	(*hash).h_mod[2] = 0;
	(*hash).h_mod[3] = 0;
	(*hash).h_mod[4] = 0;
	(*hash).h_mod[5] = 0;
	(*hash).h_mod[6] = 0;
	(*hash).h_mod[7] = 0;
	(*hash).h_mod[8] = 0;
	(*hash).h_mod[9] = 0;
	(*hash).hash_choice = hash_choice;
}

void		ft_fill_hash(t_hash *hash, int choice)
{
	if (choice == 0)
	{
		(*hash).h_mod[0] = (*hash).h[0];
		(*hash).h_mod[1] = (*hash).h[1];
		(*hash).h_mod[2] = (*hash).h[2];
		(*hash).h_mod[3] = (*hash).h[3];
		(*hash).h_mod[4] = (*hash).h[4];
		(*hash).h_mod[5] = (*hash).h[5];
		(*hash).h_mod[6] = (*hash).h[6];
		(*hash).h_mod[7] = (*hash).h[7];
	}
	else
	{
		(*hash).h[0] += (*hash).h_mod[0];
		(*hash).h[1] += (*hash).h_mod[1];
		(*hash).h[2] += (*hash).h_mod[2];
		(*hash).h[3] += (*hash).h_mod[3];
		(*hash).h[4] += (*hash).h_mod[4];
		(*hash).h[5] += (*hash).h_mod[5];
		(*hash).h[6] += (*hash).h_mod[6];
		(*hash).h[7] += (*hash).h_mod[7];
	}
}
