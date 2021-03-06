/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:42:54 by obounri           #+#    #+#             */
/*   Updated: 2022/02/11 18:04:21 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	prompt_heredoc(char *red, t_scmd *scmd)
{
	char	*heredoc;
	char	*tmp;

	heredoc = "";
	tmp = NULL;
	while (1)
	{
		tmp = readline(">");
		if (ft_strcmp(red, tmp) == 0)
			break ;
		if (!tmp)
			heredoc = ft_strjoin(heredoc, "");
		else
			heredoc = ft_strjoin(ft_strjoin(heredoc, tmp), "\n");
	}
	scmd->heredoc = ft_strdup(heredoc);
	return (0);
}

int	heredoc(char *red, t_scmd *scmd, t_env *env)
{
	char	*new_red;
	int		q;

	if (scmd->fd_infile != -10)
		close(scmd->fd_infile);
	if (scmd->heredoc)
	{
		free(scmd->heredoc);
		scmd->heredoc = NULL;
	}
	q = 0;
	if (cmp(red[0], SQ) || cmp(red[0], DQ))
	{
		new_red = trim_quotes(red);
		q = 1;
	}
	else
		new_red = red;
	prompt_heredoc(new_red, scmd);
	expand_heredoc(q, scmd, env);
	return (0);
}

void	expand_heredoc(int q, t_scmd *scmd, t_env *env)
{
	char	**t;
	int		i;

	if (!q)
	{
		i = -1;
		t = malloc(sizeof (char *) * 2);
		t[1] = NULL;
		while (scmd->heredoc[++i])
			if (scmd->heredoc[i] == '$')
				scmd->heredoc[i] = EXPAND;
		t[0] = scmd->heredoc;
		expand_vars(&t, env, 0);
		scmd->heredoc = ft_strdup(t[0]);
		free(t);
	}
}

int	trim_redir_token(char **str)
{	
	if (ft_strlen(*str) == 1 && **str <= -33)
	{
		*str += 1;
		*str = ft_strdup("");
		return (1);
	}
	return (0);
}

int	search_token(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] <= -33)
			return (1);
	return (0);
}
