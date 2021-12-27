/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 18:16:07 by obounri           #+#    #+#             */
/*   Updated: 2021/12/19 13:45:10 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char   *set_var(char **var, int status)
{
    char *tmp;

    tmp = ft_strdup(*var);
    free(*var);
    if (!ft_strcmp(tmp, "?"))
        *var = ft_itoa(status);
    else
    {
        *var = getenv(tmp);
        if (!*var)
            *var = ft_strdup("");
    }
    free(tmp);
    return (*var);
}

char *expand(char **scmd, int j, int status)
{
    char *var;
    char *tmp_scmd;
    char *first;
    char  *last;
    int tmp;

    tmp = j + 1;
    tmp_scmd = *scmd;
    first = ft_substr(tmp_scmd, 0, j++);
    // printf("[DEBUG] first [%s]\n", first); //
    if (tmp_scmd[j] == '?')
        j++;
    else
        while (tmp_scmd[j] && ft_isalnum(tmp_scmd[j]) == 1)
            j++;
    var = ft_substr(tmp_scmd, tmp, j - tmp);
    var = set_var(&var, status);
    // printf("[DEBUG] var [%s]\n", var); //
    last = ft_substr(tmp_scmd, j, ft_strlen(tmp_scmd) - j);
    // printf("[DEBUG] last [%s]\n", last); //
    free(tmp_scmd);
    tmp_scmd = NULL;
    tmp_scmd = ft_strjoin(first, var);
    tmp_scmd = ft_strjoin(tmp_scmd, last);
    free(first);
    free(last);
    return (tmp_scmd);
}

void	expand_vars(char ***scmd, int status)
{
	int i;
	int j;
	char **tmp_scmd;

	tmp_scmd = *scmd;
	i = -1;
	while (tmp_scmd[++i])
	{
		// if (tmp_scmd[i][0] == '"' || tmp_scmd[i][0] == EXPAND)
		// {
			j = 0;
			while (tmp_scmd[i][j]) 
			{
				while (tmp_scmd[i][j] && tmp_scmd[i][j] != EXPAND)
					j++;
				if (!tmp_scmd[i][j])
					break ;
				else
				{
					tmp_scmd[i] = expand(&tmp_scmd[i], j++, status);
					continue ;
				}
			}
		// }
	}
}