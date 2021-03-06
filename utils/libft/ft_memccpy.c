/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 21:31:17 by obounri           #+#    #+#             */
/*   Updated: 2019/10/27 21:56:13 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned int	i;
	char			*dst1;
	const char		*src1;

	dst1 = (char *)dst;
	src1 = (const char *)src;
	i = 0;
	while (src1[i] != '\0' && i < n)
	{
		if (src1[i] == (char)c)
		{
			dst1[i] = src1[i];
			return ((void *)&dst1[i + 1]);
		}
		dst1[i] = src1[i];
		i++;
	}
	return (0);
}
