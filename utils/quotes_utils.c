/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 19:13:43 by obounri           #+#    #+#             */
/*   Updated: 2022/01/30 19:15:26 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*trim_quotes(char *red)
{
	char	*trim_red;
	int		i;
	int		old_len;
	int		j;

	if (red[0] != '"' && red[0] != '\'')
		return (red);
	old_len = ft_strlen(red);
	trim_red = malloc(sizeof(char) * old_len - 1);
	i = 0;
	j = 1;
	while (i < old_len - 1)
	{
		trim_red[i] = red[j];
		i++;
		j++;
	}
	trim_red[old_len - 2] = '\0';
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
