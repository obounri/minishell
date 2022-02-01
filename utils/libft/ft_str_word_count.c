/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_word_count.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 15:50:05 by obounri           #+#    #+#             */
/*   Updated: 2019/11/05 19:07:55 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_str_word_count(char const *s, char c)
{
	char			*trimmed;
	char			set[2];
	unsigned int	i;
	unsigned int	wc;

	if (!s)
		return (0);
	set[0] = c;
	set[1] = '\0';
	trimmed = ft_strtrim(s, (char const *)&set);
	i = 0;
	if (ft_strlen((char const *)trimmed) == 0)
		return (i);
	wc = 1;
	while (trimmed[i] != '\0')
	{
		if (trimmed[i] == c)
			wc++;
		while (trimmed[i] == c)
			i++;
		i++;
	}
	return (wc);
}
