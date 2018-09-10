/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvignau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 15:43:10 by kvignau           #+#    #+#             */
/*   Updated: 2018/09/10 15:43:11 by kvignau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		*ft_memjoin(void const *s1, void const *s2, size_t size1,
	size_t size2)
{
	void	*join;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	else if (!s1)
		return ((void *)s2);
	else if (!s2)
		return ((void *)s1);
	join = ft_memalloc(size1 + size2);
	if (!join)
		return (NULL);
	ft_memcpy(join, s1, size1);
	ft_memcpy(join + size1, s2, size2);
	return (join);
}
