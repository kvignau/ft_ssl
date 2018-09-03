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

void		print_algo(t_hash *h, char opt)
{
	char	**algo;

	if ((opt & OPT_P) && (opt & OPT_STDIN))
		ft_printf("%s", h->file_name);
	else if (!(opt & OPT_R) && !(opt & OPT_STDIN))
	{
		if (!(opt & OPT_Q))
		{
			algo = algo_name();
			if (opt & OPT_S)
				ft_printf("%s (\"%s\") = ", algo[h->hash_choice], h->file_name);
			else
				ft_printf("%s (%s) = ", algo[h->hash_choice], h->file_name);
			free_hash_names(algo);
		}
	}
	g_print_funct[h->hash_choice](h);
	if (!(opt & OPT_STDIN) && (opt & OPT_R) && !(opt & OPT_Q))
	{
		if (opt & OPT_S)
			ft_printf(" \"%s\"", h->file_name);
		else
			ft_printf(" %s", h->file_name);
	}
	ft_putchar('\n');
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

int			ft_stdin(int i, int argc, char *opt, int hash_choice)
{
	int		ret;
	char	buf[BUFF_SIZE + 1];
	char	*str;

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
		!str ? str = ft_strdup("\0") : NULL;
		g_functions[hash_choice](str, (*opt), str, hash_choice);
		(*opt) = (*opt) & ~OPT_STDIN;
		str ? free(str) : NULL;
	}
	return (EXIT_SUCCESS);
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
