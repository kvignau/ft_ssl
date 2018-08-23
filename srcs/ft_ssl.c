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

int		print_errors(char *str)
{
	write(2, str, ft_strlen(str));
	return (EXIT_FAILURE);
}

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
	ft_putstr_fd("usage: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(" [hash] [command options] [command arguments]\n", 2);
	return (EXIT_FAILURE);
}

void	ft_init_hash(t_hash *hash, char *name)
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
}

void		ft_print_hash(t_hash *hash)
{
	int		i;
	int		j;
	uint8_t *p;

	i = 0;
	while (i < 4)
	{
		p = (uint8_t *)&hash->h[i];
		j = 0;
		while (j < 4)
		{
			ft_printf("%02x", p[j]);
			// IF LEAKS USE -> 
			// if (ft_strlen(ft_itoa_base_uimax(p[j], 16)) == 1)
			// 	ft_putchar('0');
			// ft_putstr(ft_itoa_base_uimax(p[j], 16));
			j++;
		}
		i++;
	}
}

void		print_md5(t_hash *hash, char opt)
{
	if ((opt & OPT_P) && (opt & OPT_STDIN))
			ft_printf("%s", hash->file_name);
	else if (!(opt & OPT_R) && !(opt & OPT_STDIN))
	{
		if (!(opt & OPT_Q))
		{
			if (opt & OPT_S)
				ft_printf("MD5 (\"%s\") = ", hash->file_name);
			else
				ft_printf("MD5 (%s) = ", hash->file_name);
		}
	}
	ft_print_hash(hash);
	if (!(opt & OPT_STDIN) && (opt & OPT_R) && !(opt & OPT_Q))
	{
		if (opt & OPT_S)
			ft_printf(" \"%s\"", hash->file_name);
		else
			ft_printf(" %s", hash->file_name);
	}
	ft_putchar('\n');
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

void		ft_hash_proc(t_md5 md5, t_hash *hash, char opt)
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
	print_md5(hash, opt);
}

void		ft_md5_string(char *val, char opt, char *name)
{
	t_md5	md5;
	size_t 	msg_len_bits;
	t_hash	hash;

	ft_init_hash(&hash, name);
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
	ft_hash_proc(md5, &hash, opt);
}

int			ft_open_file(int *fd, char *val)
{
	if (((*fd) = open(val, O_RDONLY)) < 0)
	{
		print_errors("ft_ssl: md5: ");
		print_errors(val);
		print_errors(": No such file or directory\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

char		*ft_files(char *val)
{
	int		fd;
	int		ret;
	char	buf[BUFF_SIZE + 1];
	char	*str;

	str = NULL;
	if (ft_open_file(&fd, val) == EXIT_FAILURE)
		return (NULL);
	while ((ret = read(fd, buf, BUFF_SIZE)) > 0)
	{
		if (ret < 0)
		{
			if (str)
				free(str);
			print_errors("Read error");
			return (NULL);
		}
		buf[ret] = '\0';
		if (!str)
			str = ft_strdup(buf);
		else
			str = ft_strjoin(str, buf);
	}
	close(fd);
	return (str);
}

void		ft_sha256_string(char *val, char opt, char *name)
{
	val = NULL;
	opt = 0;
	name = NULL;
	ft_printf("COUCOU JE SUIS UN SHA256\n");
}

int			ft_algo_choice(char *val, char *opt, int hash_choice)
{
	char	*str;

	str = NULL;
	if ((*opt) & OPT_S)
	{
		g_functions[hash_choice](val, (*opt), val);
		(*opt) = (*opt) & ~OPT_S;
	}
	else
	{
		str = ft_files(val);
		if (!str)
			str = ft_strdup("\0");
		g_functions[hash_choice](str, (*opt), val);
		free(str);
	}
	return (EXIT_SUCCESS);
}

static int	ft_check_options(char *arg, char *opt)
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
			print_errors("ft_ssl: illegal option -- ");
			print_errors(&arg[i]);
			print_errors("\nAvailable options: ft_ssl [-pqrs]\n");
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

void		free_hash_names(char **hash_names)
{
	int		i;

	i = 0;
	while (hash_names[i])
	{
		free(hash_names[i]);
		i++;
	}
	free(hash_names);
}

static int	ft_options(int *i, char *opt, int argc, char **argv)
{
	while ((*i) < argc && argv[(*i)][0] == '-')
	{
		if (argv[(*i)][1] == '\0')
			break ;
		else if (argv[(*i)][1] == '-')
		{
			(*i)++;
			break ;
		}
		else if (ft_check_options(argv[(*i)], opt) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		(*i)++;
	}
	return (EXIT_SUCCESS);
}

int			ft_stdin(int i, int argc, char *opt, int hash_choice)
{
	int ret;
	char buf[BUFF_SIZE + 1];
	char *str;

	ret = 0;
	str = NULL;
	if (i >= argc || (*opt) & OPT_P)
	{
		(*opt) = (*opt) | OPT_STDIN;
		while ((ret = read(0, buf, BUFF_SIZE)) > 0)
		{
			if (ret < 0)
			{
				return (print_errors("Read error"));
			}
			buf[ret] = '\0';
			if (!str)
				str = ft_strdup(buf);
			else
				str = ft_strjoin(str, buf);
		}
		g_functions[hash_choice](str, (*opt), str);
		(*opt) = (*opt) & ~OPT_STDIN;
	}
	return (EXIT_SUCCESS);
}

int			ft_hash_name(int *hash_choice, char *opt, char *algo)
{
	char	**hash_names;

	hash_names = NULL;
	hash_names = ft_strsplit(HASH, '|');
	while (hash_names[(*hash_choice)])
	{
		if (ft_strcmp(algo, hash_names[(*hash_choice)]) == 0)
		{
			(*opt) = (*opt) | OPT_GH;
			free_hash_names(hash_names);
			return (EXIT_SUCCESS);
		}
		(*hash_choice)++;
	}
	if (hash_names)
		free_hash_names(hash_names);
	print_errors("HASH: ");
	print_errors(algo);
	return (print_errors(" does not exist"));
}

int			main(int argc, char **argv)
{
	int		i;
	int		hash_choice;
	char	opt;

	hash_choice = 0;
	opt = 0;
	i = 2;
	if (argc < 2)
		return (print_usage(argv[0]));
	if (ft_hash_name(&hash_choice, &opt, argv[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (!(opt & OPT_GH))
		return (print_usage(argv[0]));
	if (ft_options(&i, &opt, argc, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (ft_stdin(i, argc, &opt, hash_choice) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	i--;
	while (i++ < argc - 1)
		ft_algo_choice(argv[i], &opt, hash_choice);
	return (0);
}
