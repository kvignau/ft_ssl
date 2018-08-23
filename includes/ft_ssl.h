/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvignau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 15:35:43 by kvignau           #+#    #+#             */
/*   Updated: 2018/03/23 15:35:47 by kvignau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include "../ft_printf/includes/ft_printf.h"
# include <fcntl.h>
#include <stdio.h>

# define BUFF_SIZE 1024

# define HASH "md5|sha256"

# define OPT_P (1 << 0)
# define OPT_Q (OPT_P << 1)
# define OPT_R (OPT_Q << 1)
# define OPT_S (OPT_R << 1)
# define OPT_STDIN (OPT_S << 1)
# define OPT_GH (OPT_STDIN << 1)

# define F(b, c, d) ((b & c) | (~b & d))
# define G(b, c, d) ((b & d) | (c & ~d))
# define H(b, c, d) (b ^ c ^ d)
# define I(b, c, d) (c ^ (b | ~d))
# define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

# define DEBUG 1

int32_t g_r[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17,
	22, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 4, 11, 16, 23,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10, 15, 21, 6, 10, 15, 21,
	6, 10, 15, 21, 6, 10, 15, 21};

int32_t g_k[64] = {
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

typedef struct		s_md5
{
	char			*data;
	size_t			size_all;
	size_t			message_len;
}					t_md5;

typedef struct		s_hash
{
	char			*file_name;
	int				h[8];
	int				h_mod[8];
	size_t			g;
	int				funct;
	uint32_t		*w;
	// int				lo;
	// int				hi;
}					t_hash;

typedef void	(*t_funct)(char *val, char opt, char *name);

// TO DEL
void		my_print_bits(char opt);
void    	print_bits(unsigned char octet);

int			print_usage(char *str);
int			print_errors(char *str);
void		ft_init_hash(t_hash *hash, char *name);
void		print_md5(t_hash *hash, char opt);
void		ft_fill_hash(t_hash *hash, int choice);
void		ft_rotate_md5(t_hash *hash, int i);
void		function_md5(t_hash *hash, int i);
void		ft_hash_proc(t_md5 md5, t_hash *hash, char opt);
void		ft_md5_string(char *val, char opt, char *name);
int			ft_algo_choice(char *val, char *opt, int hash_choice);
void		free_hash_names(char **hash_names);
void		ft_sha256_string(char *val, char opt, char *name);

t_funct			g_functions[] = {ft_md5_string, ft_sha256_string};

#endif
