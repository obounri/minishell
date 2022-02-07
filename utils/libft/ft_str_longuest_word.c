/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_longuest_word.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 16:01:03 by obounri           #+#    #+#             */
/*   Updated: 2019/11/06 20:37:10 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strl_longuest_word(char const *s, char c)
{
	unsigned int	count;
	unsigned int	i;
	unsigned int	lw;

	lw = 0;
	i = 0;
	while (s[i] != '\0')
	{
		count = 0;
		if (s[i] != c)
		{
			while (s[i] != c)
			{
				count++;
				i++;
			}
		}
		if (lw < count)
			lw = count;
		i++;
	}
	return (lw);
}
