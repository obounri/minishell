/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 16:50:38 by obounri           #+#    #+#             */
/*   Updated: 2022/01/19 11:44:21 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

void	add_env(t_env **env, char **var)
{
	t_env	*new_env;
	t_env	*tmp;

	new_env = malloc(sizeof(t_env) * 1);
	new_env->key = ft_strdup(var[0]);
	new_env->exp = 0;
	if (!var[1])
		new_env->value = ft_strdup("");
	else
		new_env->value = ft_strdup(var[1]);
	new_env->next = NULL;
	if (!*env)
		*env = new_env;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_env;
	}
}

char	*ft_getenv(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(key, env->key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	init(t_options *opts, char **env)
{
	int		i;
	char	**tmp;

	i = -1;
	opts->env = NULL;
	opts->status = 0;
	opts->curr_dir = getcwd(NULL, 0);
	opts->home = getenv("HOME");
	opts->prompt = NULL;
	opts->cmd = malloc(sizeof(t_cmd));
	opts->cmd->scmds = NULL;
	opts->uncqu = 0;
	opts->sh_env = env;
	opts->input = NULL;
	while (env[++i])
	{
		tmp = ft_split(env[i], '=');
		add_env(&opts->env, tmp);
		dfree(tmp);
	}
}

void	init_scmds(t_scmd *scmds, int n_scmds)
{
	int	i;

	i = 0;
	while (i < n_scmds)
	{
		scmds[i].scmd = NULL;
		scmds[i].name = NULL;
		scmds[i].exec_path = NULL;
		scmds[i].args = NULL;
		scmds[i].impld = -10;
		scmds[i].heredoc = NULL;
		scmds[i].fd_infile = -10;
		scmds[i].fd_outfile = -10;
		scmds[i].err = 0;
		i++;
	}
}
