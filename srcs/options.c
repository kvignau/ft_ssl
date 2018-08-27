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

int			ft_check_options(char *arg, char *opt)
{
	int		i;

	i = 1;
	while (arg && arg[i])
	{
		if (arg[i] == 'p')
			(*opt) = (*opt) | OPT_P;
		else if (arg[i] == 'q')
			(*opt) = (*opt) | OPT_Q;
		else if (arg[i] == 'r')
			(*opt) = (*opt) | OPT_R;
		else if (arg[i] == 's')
			(*opt) = (*opt) | OPT_S;
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

int			ft_options(int *i, char *opt, int argc, char **argv)
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
