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
# include <stdio.h>

# define BUFF_SIZE 1024

# define HASH "MD5|SHA256|BASE64"

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

typedef struct		s_algo
{
	unsigned char	*data;
	size_t			size_all;
	size_t			message_len;
}					t_algo;

typedef struct		s_hash
{
	char			*file_name;
	unsigned int	h[8];
	unsigned int	h_mod[10];
	size_t			g;
	int				funct;
	unsigned int	*w;
	int				hash_choice;
}					t_hash;

typedef struct		s_opts
{
	int				opt;
	int				len;
}					t_opts;

typedef void		(*t_funct)(char *val, t_opts opt, char *name,
	int hash_choice);
typedef void		(*t_algo_print)(t_hash *hash);

void				print_algo(t_hash *hash, t_opts opt);
int					ft_algo_choice(char *val, t_opts *opt, int hash_choice);
int					ft_stdin(int i, int argc, t_opts *opt, int hash_choice);
char				**algo_name(void);

void				ft_md5_string(char *val, t_opts opt, char *name,
	int hash_choice);
void				ft_print_hash_md5(t_hash *hash);

void				ft_print_hash_sha256(t_hash *hash);
void				ft_sha256_string(char *val, t_opts opt, char *name,
	int hash_choice);

uint64_t			swap_uint64(uint64_t val);
int					ft_open_file(int *fd, char *val);
int					ft_files(char *val, char **str, t_opts *opt);
int					print_usage(char *str);
int					print_errors(char *str);

void				ft_init_hash_md5(t_hash *hash, char *name, int hash_choice);
void				ft_init_hash_sha256(t_hash *hash, char *name,
	int hash_choice);
void				ft_fill_hash(t_hash *hash, int choice);
void				ft_init_message(t_algo *md5, char *val, t_opts opt);

t_opts				ft_init_opts(void);
int					ft_check_options(char *arg, t_opts *opt);
void				free_hash_names(char **hash_names);
int					ft_options(int *i, t_opts *opt, int argc, char **argv);
int					ft_hash_name(int *hash_choice, t_opts *opt, char *algo);

#endif
