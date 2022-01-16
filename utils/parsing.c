/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 16:57:23 by obounri           #+#    #+#             */
/*   Updated: 2022/01/16 17:46:20 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_exec_path(t_options *opts, char *name)
{
	int				i;
	DIR				*dp;
	struct dirent	*dirp;
	char			*var;

	var = ft_getenv(opts->env, "PATH");
	if (!var)
		return (NULL);
	opts->path = ft_split(var, ':');
	i = -1;
	while (opts->path[++i])
	{
		dp = opendir(opts->path[i]);
		if (!dp)
			continue ;
		dirp = readdir(dp);
		while (dirp != NULL)
		{
			if (ft_strcmp(dirp->d_name, name) == 0)
				return (ft_strjoin(ft_strjoin(opts->path[i], "/"), name));
			dirp = readdir(dp);
		}
		closedir(dp);
	}
	return (NULL);
}

void	init_for_exec(t_options *opts, int i, char **scmd)
{
	int	h;

	h = -1;
	while (scmd && scmd[++h])
		scmd[h] = trim_quotes(scmd[h]);
	opts->cmd->scmds[i].impld = is_impld(scmd[0]);
	if (opts->cmd->scmds[i].impld < 0)
	{
		if (open(scmd[0], O_RDONLY) != -1)
			opts->cmd->scmds[i].exec_path = scmd[0];
		else
			opts->cmd->scmds[i].exec_path = find_exec_path(opts, scmd[0]);
	}
	opts->cmd->scmds[i].name = scmd[0];
	opts->cmd->scmds[i].args = &scmd[0];
}

void	parse_scmds(t_options *opts, char **scmds)
{
	int		i;
	char	**split_scmd;

	i = -1;
	while (++i < opts->cmd->n_scmds)
	{
		split_scmd = ft_split(scmds[i], UNQSPACE);
		// if (!split_scmd) // ??
		// 	break; // ??
		expand_vars(&split_scmd, opts->env, opts->status);
		if (!redirect(&split_scmd, &opts->cmd->scmds[i], opts->env))
		{
			opts->cmd->scmds[i].err = 1;
			continue ;
		}
		new_alloc(&split_scmd);
		// if (!split_scmd) // ??
		// 	break; // ??
		init_for_exec(opts, i, split_scmd);
	}
}

int	parse_input(t_options *opts)
{
	t_quote	*quotes;
	char	**scmds;

	scmds = NULL;
	if (!opts->input[0])
		return (0);
	add_history(opts->input);
	quotes = check_quotes_pipes(opts);
	if (opts->uncqu == 1)
	{
		opts->uncqu = 0;
		return (0);
	}
	if (!check_errors(opts))
		return (0);
	scmds = ft_split(opts->input, PIPE);
	if (check_scmds(scmds) == 0)
		return (0);
	while (scmds[opts->cmd->n_scmds])
		opts->cmd->n_scmds++;
	opts->cmd->scmds = malloc(sizeof(t_scmd) * (opts->cmd->n_scmds));
	init_scmds(opts->cmd->scmds, opts->cmd->n_scmds);
	parse_scmds(opts, scmds);
	return (1);
}
