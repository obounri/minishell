/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 18:16:07 by obounri           #+#    #+#             */
/*   Updated: 2022/01/30 19:38:04 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*set_var(char **var, t_env *env, int status)
{
	char	*tmp;

	tmp = ft_strdup(*var);
	free(*var);
	if (!ft_strcmp(tmp, "?"))
	{
		if (WIFSIGNALED(status))
			*var = ft_itoa(128 + WTERMSIG(status));
		else
			*var = ft_itoa(WEXITSTATUS(status));
	}
	else
	{
		*var = ft_strdup(ft_getenv(env, tmp));
		if (!*var)
			*var = ft_strdup("");
	}
	free(tmp);
	return (*var);
}

char	*expand(char **scmd, int j, t_env *env, int status)
{
	char	*var;
	char	*tmp_scmd;
	char	*first;
	char	*last;
	int		tmp;

	tmp = j + 1;
	tmp_scmd = *scmd;
	first = ft_substr(tmp_scmd, 0, j++);
	if (tmp_scmd[j] == '?')
		j++;
	else
		while (tmp_scmd[j] && (ft_isalnum(tmp_scmd[j]) == 1
				|| tmp_scmd[j] == '_'))
			j++;
	var = ft_substr(tmp_scmd, tmp, j - tmp);
	var = set_var(&var, env, status);
	last = ft_substr(tmp_scmd, j, ft_strlen(tmp_scmd) - j);
	expand_2(&tmp_scmd, &first, &var, &last);
	return (tmp_scmd);
}

void	expand_2(char **tmp_scmd, char **first, char **var, char **last)
{
	free(*tmp_scmd);
	*tmp_scmd = NULL;
	*tmp_scmd = ft_strjoin(*first, *var);
	*tmp_scmd = ft_strjoin(*tmp_scmd, *last);
	free(*first);
	free(*var);
	free(*last);
}

void	expand_vars(char ***scmd, t_env *env, int status)
{
	int		i;
	int		j;
	char	**tmp_scmd;

	tmp_scmd = *scmd;
	i = -1;
	while (tmp_scmd[++i])
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
				tmp_scmd[i] = expand(&tmp_scmd[i], j++, env, status);
				continue ;
			}
		}
	}
}
