/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 13:05:33 by obounri           #+#    #+#             */
/*   Updated: 2022/01/18 14:04:18 by obounri          ###   ########.fr       */
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

void	free_scmds(t_scmd **scmds)
{
	free(*scmds);
	*scmds = NULL;
}

void	clean_exit(t_options *opts, int code)
{
	t_env *next;

	free(opts->cmd);
	if (opts->cmd->scmds)
		free_scmds(&opts->cmd->scmds);
	next = opts->env;
	while(opts->env)
	{
		next = opts->env->next;
		free(opts->env);
		opts->env = next;
	}
	exit(code);
}