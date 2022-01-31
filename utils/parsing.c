/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 16:57:23 by obounri           #+#    #+#             */
/*   Updated: 2022/01/30 19:24:19 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*exec_path(DIR *dir, char **paths, int i, char *name)
{
	char	*tmp;
	char	*ex_p;

	tmp = ft_strjoin(paths[i], "/");
	ex_p = ft_strjoin(tmp, name);
	free(tmp);
	dfree(paths);
	closedir(dir);
	return (ex_p);
}

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
				return (exec_path(dp, opts->path, i, name));
			dirp = readdir(dp);
		}
		closedir(dp);
	}
	return (NULL);
}

void	init_for_exec(t_options *opts, int i)
{
	int		h;
	char	**tmp;

	tmp = opts->cmd->scmds[i].scmd;
	h = -1;
	while (tmp && tmp[++h])
		tmp[h] = trim_quotes(tmp[h]);
	opts->cmd->scmds[i].impld = is_impld(tmp[0]);
	if (opts->cmd->scmds[i].impld < 0)
	{
		if (open(tmp[0], O_RDONLY) != -1)
			opts->cmd->scmds[i].exec_path = ft_strdup(tmp[0]);
		else
			opts->cmd->scmds[i].exec_path = find_exec_path(opts, tmp[0]);
	}
	opts->cmd->scmds[i].name = tmp[0];
	opts->cmd->scmds[i].args = &tmp[0];
}

void	parse_scmds(t_options *opts, char **scmds)
{
	int	i;

	while (scmds[opts->cmd->n_scmds])
		opts->cmd->n_scmds++;
	opts->cmd->scmds = malloc(sizeof(t_scmd) * (opts->cmd->n_scmds));
	init_scmds(opts->cmd->scmds, opts->cmd->n_scmds);
	i = -1;
	while (++i < opts->cmd->n_scmds)
	{
		opts->cmd->scmds[i].scmd = ft_split(scmds[i], UNQSPACE);
		expand_vars(&opts->cmd->scmds[i].scmd, opts->env, opts->status);
		if (!redirect(&opts->cmd->scmds[i], opts->env))
		{
			opts->cmd->scmds[i].err = 1;
			continue ;
		}
		new_alloc(&opts->cmd->scmds[i].scmd);
		init_for_exec(opts, i);
	}
}

int	parse_input(t_options *opts)
{
	t_quote	*quotes;
	char	**scmds;

	if (!opts->input[0])
		return (0);
	add_history(opts->input);
	quotes = check_quotes_pipes(opts);
	if (opts->uncqu == 1)
	{
		free_quotes(quotes);
		opts->uncqu = 0;
		return (0);
	}
	free_quotes(quotes);
	if (!check_errors(opts))
		return (0);
	scmds = ft_split(opts->input, PIPE);
	if (check_scmds(scmds) == 0)
	{
		dfree(scmds);
		return (0);
	}
	parse_scmds(opts, scmds);
	dfree(scmds);
	return (1);
}
