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
# define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
# define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
# define SHR(x, n) ((x) >> n)
# define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
# define A(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
# define B(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
# define C(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3))
# define D(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

# define DEBUG 1

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

typedef struct		s_md5
{
	unsigned char	*data;
	size_t			size_all;
	size_t			message_len;
}					t_md5;

typedef struct		s_hash
{
	char			*file_name;
	unsigned int	h[8];
	unsigned int	h_mod[10];
	size_t			g;
	int				funct;
	unsigned int	*w;
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
int			ft_stdin(int i, int argc, char *opt, int hash_choice);
int			ft_hash_name(int *hash_choice, char *opt, char *algo);
int			ft_files(char *val, char **str);

t_funct			g_functions[] = {ft_md5_string, ft_sha256_string};

#endif
