/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 13:05:33 by obounri           #+#    #+#             */
/*   Updated: 2022/01/18 17:37:44 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	dfree(char **str)
{
	int	i;

	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
}

void	free_scmds(t_scmd **scmds, int n_scmds)
{
	int i;

	i = -1;
	while (++i < n_scmds)
	{
		free((*scmds)[i].exec_path);
		dfree((*scmds)[i].scmd);
	}
	free(*scmds);
	*scmds = NULL;
}

void	clean_exit(t_options *opts, int code)
{
	t_env *next;

	if (opts->cmd->scmds)
		free_scmds(&opts->cmd->scmds, opts->cmd->n_scmds);
	free(opts->cmd);
	next = opts->env;
	while(opts->env)
	{
		next = opts->env->next;
		free(opts->env->key);
		free(opts->env->value);
		free(opts->env);
		opts->env = next;
	}
	if (opts->prompt)
		free(opts->prompt);
	if (opts->input)
		free(opts->input);
	if (opts->curr_dir)
		free(opts->curr_dir);
	if (opts->path)
		free(opts->path);
	if (opts->home)
		free(opts->home);
	exit(code);
}