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

char		**algo_name(void)
{
	char	**hash_names;

	hash_names = NULL;
	hash_names = ft_strsplit(HASH, '|');
	return (hash_names);
}

void		print_algo(t_hash *h, t_opts opt)
{
	char	**algo;

	if ((opt.opt & OPT_P) && (opt.opt & OPT_STDIN))
		ft_printf("%s", h->file_name);
	else if (!(opt.opt & OPT_R) && !(opt.opt & OPT_STDIN))
	{
		if (!(opt.opt & OPT_Q))
		{
			algo = algo_name();
			if (opt.opt & OPT_S)
				ft_printf("%s (\"%s\") = ", algo[h->hash_choice], h->file_name);
			else
				ft_printf("%s (%s) = ", algo[h->hash_choice], h->file_name);
			free_hash_names(algo);
		}
	}
	g_print_funct[h->hash_choice](h);
	if (!(opt.opt & OPT_STDIN) && (opt.opt & OPT_R) && !(opt.opt & OPT_Q))
	{
		if (opt.opt & OPT_S)
			ft_printf(" \"%s\"", h->file_name);
		else
			ft_printf(" %s", h->file_name);
	}
	ft_putchar('\n');
}

int			ft_algo_choice(char *val, t_opts *opt, int hash_choice)
{
	char	*str;

	str = NULL;
	if (opt->opt & OPT_S)
	{
		opt->len = ft_strlen(val);
		g_functions[hash_choice](val, *opt, val, hash_choice);
		opt->opt = opt->opt & ~OPT_S;
		opt->len = 0;
	}
	else
	{
		if (ft_files(val, &str, opt) == EXIT_FAILURE)
		{
			if (str)
				free(str);
			return (EXIT_FAILURE);
		}
		if (!str)
			str = ft_strdup("\0");
		g_functions[hash_choice](str, *opt, val, hash_choice);
		opt->len = 0;
		free(str);
	}
	return (EXIT_SUCCESS);
}

int			ft_stdin(int i, int argc, t_opts *opt, int hash_choice)
{
	int		ret;
	char	buf[BUFF_SIZE + 1];
	char	*str;

	ret = 0;
	str = NULL;
	if (i >= argc || opt->opt & OPT_P)
	{
		opt->opt = opt->opt | OPT_STDIN;
		while ((ret = read(0, buf, BUFF_SIZE)) > 0)
		{
			buf[ret] = '\0';
			if (!str)
				str = ft_strdup(buf);
			else
				str = ft_strjoinandfree(str, buf, 1);
			opt->len += ret;
		}
		!str ? str = ft_strdup("\0") : NULL;
		g_functions[hash_choice](str, *opt, str, hash_choice);
		opt->opt = opt->opt & ~OPT_STDIN;
		opt->len = 0;
		str ? free(str) : NULL;
	}
	return (EXIT_SUCCESS);
}

int			main(int argc, char **argv)
{
	int		i;
	int		hash_choice;
	t_opts	opt;

	hash_choice = 0;
	opt = ft_init_opts();
	i = 2;
	if (argc < 2)
		return (print_usage(argv[0]));
	if (ft_hash_name(&hash_choice, &opt, argv[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (!(opt.opt & OPT_GH))
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
