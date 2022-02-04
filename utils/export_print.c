/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 19:31:58 by obounri           #+#    #+#             */
/*   Updated: 2022/01/30 19:32:40 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	export_print(t_env *env)
{
	while (env)
	{
		if (!env->exp)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			printf("declare -x %s\n", env->key);
		env = env->next;
	}
}

int	get_key_value(char *args, t_env **env)
{
	char	*key;
	char	*value;
	int		j;
	int		equal;

	equal = 0;
	j = -1;
	while (args[++j])
	{
		if (args[j] == '=')
		{
			equal = 1;
			key = ft_substr(args, 0, j);
			value = ft_substr(args, j + 1, ft_strlen(args));
			if (value && (value[0] == '"' || value[0] == '\''))
				value = trim_quotes(value);
			if (!already_exist(key, env))
				modify_var(key, value, env);
			else
				add_var(key, value, env, 0);
		}
	}
	no_equal(equal, key, args, env);
}

void	no_equal(int equal, char *key, char *args, t_env **env)
{
	if (!equal)
	{
		key = ft_strdup(args);
		if (already_exist(key, env))
			add_var(key, NULL, env, 1);
	}
}
