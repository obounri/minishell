/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 19:13:43 by obounri           #+#    #+#             */
/*   Updated: 2022/02/11 16:58:22 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	new_size(char *red)
{
	int	i;
	int	size;

	size = 0;
	i = 0;
	while (red && red[i])
	{
		if (cmp(red[i], SQ) || cmp(red[i],DQ))
		{
			i++;
			continue ;
		}
		size++;
		i++;
	}
	return (size);
}

char	*trim_quotes(char *red)
{
	char	*trim_red;
	int		i;
	int		j;
	int		size;

	size = new_size(red);
	trim_red = malloc(sizeof(char) * (size + 1));
	i = 0;
	j = 0;
	while (j < size)
	{
		if (cmp(red[i], SQ) || cmp(red[i], DQ))
		{
			i++;
			continue;
		}
		trim_red[j] = red[i];
		j++;
		i++;
	}
	trim_red[j] = '\0';
	free(red);
	return (trim_red);
}

void	unclosed_quotes(t_quote *quotes, t_options *opts)
{
	if (quoted(quotes, 0))
	{
		ft_error("minishell", NULL, ": Unclosed quotes");
		opts->uncqu = 1;
		opts->status = 511;
	}
}
