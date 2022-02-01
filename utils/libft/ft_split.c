/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/01 15:12:40 by obounri           #+#    #+#             */
/*   Updated: 2022/01/19 11:43:57 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	word_length(char const *s, char c)
{
	unsigned int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] != '\0' && !(s[i] == c))
		i++;
	return (i);
}

size_t	str_word_count(char const *s, char c)
{
	char			*trimmed;
	char			set[2];
	unsigned int	i;
	unsigned int	wc;

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
	free(trimmed);
	return (wc);
}

void	freetabs(char **splited, unsigned int index)
{
	unsigned int	i;

	i = 0;
	while (i < index)
	{
		free(splited[i]);
		i++;
	}
	free(splited);
	splited = NULL;
}

char	**aammer(char **splited, char const *s, char c, unsigned int i)
{
	unsigned int	index;
	unsigned int	len;

	index = 0;
	while (s[i] != '\0' && (index < str_word_count(s, c)))
	{
		while ((s[i] == c) && s[i] != '\0')
			i++;
		len = word_length(&s[i], c);
		splited[index] = ft_substr(s, i, len);
		if (!(splited))
		{
			freetabs(splited, index);
			return (0);
		}
		index++;
		i += len;
	}
	splited[index] = 0;
	return (splited);
}

char	**ft_split(char const *s, char c)
{
	char			**splited;
	unsigned int	i;

	splited = (char **)malloc((str_word_count(s, c) + 1) * sizeof(char *));
	if (!s || !(splited))
		return (0);
	i = 0;
	splited = aammer(splited, s, c, i);
	if (!(splited))
		return (0);
	return (splited);
}
