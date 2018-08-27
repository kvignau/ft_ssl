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

t_funct			g_functions[] = {ft_md5_string, ft_sha256_string};
t_algo_print	g_print_funct[] = {ft_print_hash_md5, ft_print_hash_sha256};

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

void	ft_init_hash_md5(t_hash *hash, char *name, int hash_choice)
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

void	ft_init_hash_sha256(t_hash *hash, char *name, int hash_choice)
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
			// ft_printf("%02x", p[j]);
			// IF LEAKS USE ->
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

char		**algo_name()
{
	char	**hash_names;

	hash_names = NULL;
	hash_names = ft_strsplit(HASH, '|');
	return (hash_names);
}

void		print_algo(t_hash *hash, char opt)
{
	char	**algo;

	if ((opt & OPT_P) && (opt & OPT_STDIN))
			ft_printf("%s", hash->file_name);
	else if (!(opt & OPT_R) && !(opt & OPT_STDIN))
	{
		if (!(opt & OPT_Q))
		{
			algo = algo_name();
			if (opt & OPT_S)
				ft_printf("%s (\"%s\") = ", algo[hash->hash_choice], hash->file_name);
			else
				ft_printf("%s (%s) = ", algo[hash->hash_choice], hash->file_name);
			free_hash_names(algo);
		}
	}
	g_print_funct[hash->hash_choice](hash);
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

void		ft_init_message(t_algo *md5, char *val)
{
	md5->message_len = ft_strlen(val);
	md5->size_all = md5->message_len + 1;
	while (md5->size_all % 64 != 56)
		md5->size_all++;
	md5->size_all += 8;
	md5->data = (unsigned char *)malloc(sizeof(unsigned char) * md5->size_all);
	ft_memcpy(md5->data, val, md5->message_len);
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

int		ft_files(char *val, char **str)
{
	int		fd;
	int		ret;
	char	buf[BUFF_SIZE + 1];

	if (ft_open_file(&fd, val) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while ((ret = read(fd, buf, BUFF_SIZE)) > 0)
	{
		if (ret < 0)
			return (print_errors("Read error"));
		buf[ret] = '\0';
		if (!(*str))
			(*str) = ft_strdup(buf);
		else
			(*str) = ft_strjoinandfree((*str), buf, 1);
	}
	close(fd);
	return (EXIT_SUCCESS);
}

uint64_t	swap_uint64(uint64_t val)
{
	val = ((val << 8) & 0xFF00FF00FF00FF00ULL) |
	((val >> 8) & 0x00FF00FF00FF00FFULL);
	val = ((val << 16) & 0xFFFF0000FFFF0000ULL) |
	((val >> 16) & 0x0000FFFF0000FFFFULL);
	return (val << 32) | (val >> 32);
}

int			ft_algo_choice(char *val, char *opt, int hash_choice)
{
	char	*str;

	str = NULL;
	if ((*opt) & OPT_S)
	{
		g_functions[hash_choice](val, (*opt), val, hash_choice);
		(*opt) = (*opt) & ~OPT_S;
	}
	else
	{
		if (ft_files(val, &str) == EXIT_FAILURE)
		{
			if (str)
				free(str);
			return (EXIT_FAILURE);
		}
		if (!str)
			str = ft_strdup("\0");
		g_functions[hash_choice](str, (*opt), val, hash_choice);
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
				return (print_errors("Read error"));
			buf[ret] = '\0';
			if (!str)
				str = ft_strdup(buf);
			else
				str = ft_strjoin(str, buf);
		}
		g_functions[hash_choice](str, (*opt), str, hash_choice);
		(*opt) = (*opt) & ~OPT_STDIN;
		if (str)
			free(str);
	}
	return (EXIT_SUCCESS);
}

int			ft_hash_name(int *hash_choice, char *opt, char *algo)
{
	char	**hash_names;

	hash_names = NULL;
	hash_names = ft_strsplit(HASH, '|');
	algo = ft_strupper(algo);
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
