/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:42:36 by obounri           #+#    #+#             */
/*   Updated: 2022/01/18 15:47:31 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	search_token(char *str)
{
	int i;

	i = -1;
	while (str[++i])
		if (str[i] <= -33)
			return(1);
	return (0);
}

char *save_remains(char **str)
{
	int i;
	char *remains;
	char *tmp;

	tmp = *str;
	i = 0;
	if (tmp[0] <= -33)
		return (NULL);
	while (tmp[i] && tmp[i] > -33)
		i++;
	remains = ft_substr(tmp, 0, i);
	while (i--)
		*str += 1;
	return (remains);
}

int	handle_in(char **str, t_scmd *scmd, int token, t_env *env)
{
	int i;
	char *tmp;

	i = 0;
	tmp = *str;
	while (tmp[i] && tmp[i] > -33)
		i++;
	if (!redirect_type(ft_substr(*str, 0, i), scmd, token, env))
		return (0);
	while (i--)
		*str += 1;
	return (1);
}

int		detect_redir(char **str, t_scmd *scmd, int *token, t_env *env)
{
	int i;
	char *tmp;

	while (**str)
	{
		i = 0;
		*token = **str;
		if (ft_strlen(*str) == 1 && **str <= -33)
		{
			*str += 1;
			*str = ft_strdup("");
			return (1);
		}
		*str += 1;
		tmp = *str;
		while (tmp[i] && tmp[i] > -33)
			i++;
		if (!redirect_type(ft_substr(*str, 0, i), scmd, *token, env))
			return (-1);
		while (i--)
			*str += 1;
	}
	*str = ft_strdup("");
	return (0);
}

int		redirect(t_scmd *cmd, t_env *env)
{
	int i;
	int in;
	char **tmp;
	char *remains;
	int token;

	i = -1;
	in = 0;
	tmp = cmd->scmd;
	while (tmp[++i])
	{
		remains = NULL;
		if(!in && !search_token(tmp[i]))
			continue ;
		remains = NULL;
		if (!in)
			remains = save_remains(&tmp[i]);
		if (in)
			if (!handle_in(&tmp[i], cmd, token, env))
				return (0);
		in = detect_redir(&tmp[i], cmd, &token, env);
		if (in == -1)
		{
			if (remains)
				free(remains);
			return (0);
		}
		if (remains)
		{
			tmp[i] = ft_strdup(remains);
			free(remains);
		}
	}
	return (1);
}