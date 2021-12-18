/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 18:16:07 by obounri           #+#    #+#             */
/*   Updated: 2021/12/18 18:16:29 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *expand(char **scmd, int j)
{
    char *var;
    char *tmp_scmd;
    char *first;
    char  *last;
    int tmp;

    tmp = j + 1;
    tmp_scmd = *scmd;
    first = ft_substr(tmp_scmd, 0, j);
    // printf("[DEBUG] first [%s]\n", first); //
    j++;
    while (tmp_scmd[j] && ft_isalnum(tmp_scmd[j]) == 1)
        j++;
    var = getenv(ft_substr(tmp_scmd, tmp, j - tmp));
    // printf("[DEBUG] var [%s]\n", var); //
    last = ft_substr(tmp_scmd, j, ft_strlen(tmp_scmd) - j);
    // printf("[DEBUG] last [%s]\n", last); //
    free(tmp_scmd);
    tmp_scmd = NULL;
    if (!var)
        tmp_scmd = ft_strjoin(first, "");
    else
        tmp_scmd = ft_strjoin(first, var);
    tmp_scmd = ft_strjoin(tmp_scmd, last);
    free(first);
    free(last);
    return (tmp_scmd);
}

void	expand_redirect(char ***scmd)
{
	int i;
	int j;
	char **tmp_scmd;

	tmp_scmd = *scmd;
	i = -1;
	while (tmp_scmd[++i])
	{
		if (tmp_scmd[i][0] == '"' || tmp_scmd[i][0] == EXPAND)
		{
			j = 0;
			while (tmp_scmd[i][j]) 
			{
				while (tmp_scmd[i][j] && tmp_scmd[i][j] != EXPAND)
					j++;
				if (!tmp_scmd[i][j])
					break ;
				else
				{
					tmp_scmd[i] = expand(&tmp_scmd[i], j++);
					continue ;
				}
			}
		}
	}
}