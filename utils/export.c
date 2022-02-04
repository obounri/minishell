/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 19:30:53 by obounri           #+#    #+#             */
/*   Updated: 2022/01/30 19:31:25 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	already_exist(char *key, t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

void	add_var(char *key, char *value, t_env **env, int exp)
{
	t_env	*tmp;
	t_env	*new_var;

	new_var = malloc(sizeof(t_env));
	new_var->key = key;
	new_var->value = value;
	new_var->exp = exp;
	new_var->next = NULL;
	if (!*env)
		*env = new_var;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_var;
	}
}

void	modify_var(char *key, char *value, t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			free(tmp->value);
			tmp->value = value;
			if (tmp->exp == 1)
				tmp->exp = 0;
			break ;
		}
		tmp = tmp->next;
	}
}

int	check_export_args(char *arg)
{
	int	i;

	i = 0;
	if (arg && arg[0] == '=')
		return (0);
	while (arg && arg[i] && arg[i] != '=')
	{
		if (ft_isdigit(arg[i]) && i == 0)
			return (0);
		if (!ft_isdigit(arg[i]) && !ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	export(char **args, t_env **env, int child)
{
	int		i;

	i = 0;
	if (!*env && child)
		exit(EXIT_FAILURE);
	if (!args[1])
		export_print(*env);
	while (args[++i])
	{
		if (!check_export_args(args[i]))
			ft_error("minishell: `", args[i], "': not a valid identifier\n");
		else
			get_key_value(args[i], env);
	}
	if (child)
		exit(EXIT_SUCCESS);
}
