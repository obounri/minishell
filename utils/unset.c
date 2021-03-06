/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 18:58:49 by obounri           #+#    #+#             */
/*   Updated: 2022/02/11 20:06:53 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	unset_var(t_env **env, char *arg)
{
	t_env	*tmp_env;
	t_env	*prev;

	tmp_env = *env;
	prev = NULL;
	while (tmp_env)
	{
		if (ft_strcmp(tmp_env->key, arg) == 0)
		{
			if (prev)
				prev->next = tmp_env->next;
			else
			{
				if (tmp_env->next)
					*env = tmp_env->next;
				else
					*env = NULL;
			}
			return ;
		}
		prev = tmp_env;
		tmp_env = tmp_env->next;
	}
}

void	unset(t_options *opts, char **args, int child)
{
	int	i;
	int	j;

	i = -1;
	while (args[++i])
	{
		j = 0;
		while (args[i][j] && (ft_isalnum(args[i][j]) == 1 || args[i][j] == '_'))
			j++;
		if (args[i][j])
		{
			ft_error("minishell: unset: `",
				args[i], "': not a valid identifier");
			opts->status = 1;
			continue ;
		}
		unset_var(&opts->env, args[i]);
	}
	opts->status = 0;
	if (child)
		exit(EXIT_SUCCESS);
}

void	ft_exit(t_options *opts, char **args, int *status)
{
	int	i;

	i = 0;
	while (args[0] && ft_isdigit(args[0][i]))
		i++;
	if (args[0] && args[0][i])
	{
		ft_error("minishell: exit: ", args[0], ": numeric argument required");
		clean_exit(opts, 255);
	}
	i = 0;
	while (args[i])
		i++;
	if (i == 0)
		clean_exit(opts, 0);
	else if (i > 1)
	{
		ft_error("minishell: exit", NULL, ": too many arguments");
		*status = 512;
	}
	else
		clean_exit(opts, ft_atoi(args[0]));
}

int	ft_error(char *prob, char *var, char *err)
{
	ft_putstr_fd(prob, 2);
	if (var)
		ft_putstr_fd(var, 2);
	ft_putstr_fd(err, 2);
	ft_putstr_fd("\n", 2);
	return (0);
}
