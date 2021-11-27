/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 19:51:46 by obounri           #+#    #+#             */
/*   Updated: 2019/10/27 16:59:13 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char p;
	unsigned char *ps;

	p = (unsigned char)c;
	ps = (unsigned char *)s;
	while (n--)
	{
		if (*ps == p)
			return ((void *)ps);
		ps++;
	}
	return (0);
}
