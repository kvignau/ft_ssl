/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvignau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/27 20:01:14 by kvignau           #+#    #+#             */
/*   Updated: 2018/08/27 20:01:17 by kvignau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

t_opts		ft_init_opts(void)
{
	t_opts	opt;

	opt.opt = 0;
	opt.fd = 1;
	opt.output = NULL;
	opt.input = NULL;
	opt.len = 0;
	return (opt);
}

int			ft_check_options(char *arg, t_opts *opt)
{
	int		i;

	i = 1;
	while (arg && arg[i])
	{
		if (arg[i] == 'p')
			opt->opt = opt->opt | OPT_P;
		else if (arg[i] == 'q')
			opt->opt = opt->opt | OPT_Q;
		else if (arg[i] == 'r')
			opt->opt = opt->opt | OPT_R;
		else if (arg[i] == 's')
			opt->opt = opt->opt | OPT_S;
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

int			ft_created_file(t_opts *opt)
{
	if ((opt->fd = open(opt->output, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
	{
		print_errors("ft_ssl: ");
		print_errors(opt->output);
		print_errors(": Error creating file\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int			ft_next_arg(char *arg, t_opts *opt)
{
	if ((opt->opt & OPT_O) && !opt->output)
	{
		opt->output = strdup(arg);
		return (EXIT_SUCCESS);
	}
	if ((opt->opt & OPT_I) && !opt->input)
	{
		opt->input = strdup(arg);
		return (EXIT_SUCCESS);
	}
	return (print_errors("Options error\n"));
}

int			ft_check_des(char *arg, t_opts *opt)
{
	int		i;

	i = 0;
	while (arg && arg[++i])
	{
		if (arg[i] == 'd')
			opt->opt = opt->opt | OPT_D;
		else if (arg[i] == 'e')
			opt->opt = opt->opt | OPT_E;
		else if (arg[i] == 'i')
			opt->opt = opt->opt | OPT_I;
		else if (arg[i] == 'o')
			opt->opt = opt->opt | OPT_O;
		else
		{
			print_errors("ft_ssl: illegal option -- ");
			print_errors(&arg[i]);
			print_errors("\nAvailable options: ft_ssl hash [-deio]\n");
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int			ft_options_des(int *i, t_opts *opt, int argc, char **argv)
{
	while ((*i) < argc)
	{
		if (argv[(*i)][0] == '-')
		{
			if (argv[(*i)][1] == '\0' || argv[(*i)][1] == '-')
				break ;
			if (ft_check_des(argv[(*i)], opt) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		else
		{
			if (ft_next_arg(argv[(*i)], opt))
				return (EXIT_FAILURE);
		}
		(*i)++;
	}
	if (((opt->opt & OPT_O) && !opt->output) ||
		((opt->opt & OPT_I) && !opt->input))
		return (print_errors("Options error\n"));
	return (opt->opt & OPT_O ? (ft_created_file(opt)) : (EXIT_SUCCESS));
}

int			ft_options(int *i, t_opts *opt, int argc, char **argv)
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

int			ft_hash_name(int *hash_choice, t_opts *opt, char *algo)
{
	char	**hash_names;

	hash_names = NULL;
	hash_names = ft_strsplit(HASH, '|');
	algo = ft_strupper(algo);
	while (hash_names[(*hash_choice)])
	{
		if (ft_strcmp(algo, hash_names[(*hash_choice)]) == 0)
		{
			if ((*hash_choice) > 1)
				opt->opt = opt->opt | OPT_DES;
			opt->opt = opt->opt | OPT_GH;
			free_hash_names(hash_names);
			return (EXIT_SUCCESS);
		}
		(*hash_choice)++;
	}
	if (hash_names)
		free_hash_names(hash_names);
	print_errors("HASH: ");
	print_errors(algo);
	print_errors(" does not exist\nAvailable hash [");
	print_errors(HASH);
	return (print_errors("]\n"));
}
