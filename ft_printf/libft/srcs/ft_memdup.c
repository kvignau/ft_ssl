/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvignau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 15:29:31 by kvignau           #+#    #+#             */
/*   Updated: 2018/09/10 15:29:34 by kvignau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void				*ft_memdup(const void *src, size_t n)
{
	void	*tab;
	size_t	i;

	i = 0;
	tab = ft_memalloc(n);
	if (!tab)
		return (NULL);
	return (ft_memcpy(tab, src, n));
}
