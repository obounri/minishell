/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 19:22:12 by obounri           #+#    #+#             */
/*   Updated: 2022/01/30 19:23:44 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	redirect_type(char *red, t_scmd *scmd, int type, t_env *env)
{
	if (type == IN)
	{
		if (!in(red, scmd))
			return (0);
	}
	else if (type == OUT)
	{
		if (!out(red, scmd))
			return (0);
	}
	else if (type == HEREDOC)
		heredoc(red, scmd, env);
	else if (type == APPEND)
	{
		if (!append(red, scmd))
			return (0);
	}
	return (1);
}

int	new_alloc_size(char **cmd)
{
	int	i;
	int	new_size;

	i = 0;
	new_size = 0;
	while (cmd && cmd[i])
	{
		if (is_empty(cmd[i]))
			new_size++;
		i++;
	}
	return (new_size);
}

int	tab_len(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
		i++;
	return (i);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab && tab[i])
		{
			free(tab[i]);
			i++;
		}
		free(tab);
	}
}

void	new_alloc(char ***cmd)
{
	int		i;
	int		j;
	char	**new;
	int		size;

	i = -1;
	j = -1;
	size = new_alloc_size(*cmd);
	new = malloc(sizeof(char *) * size + 1);
	while ((*cmd)[++i])
	{
		if (ft_strcmp((*cmd)[i],""))
			new[++j] = (*cmd)[i];
		else
			free((*cmd)[i]);
	}
	new[++j] = NULL;
	free(*cmd);
	*cmd = NULL;
	*cmd = new;
}
