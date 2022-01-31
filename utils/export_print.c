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

void	sort(t_env **en)
{
	char	*tmp;
	t_env	*current_node;

	current_node = *en;
	while (current_node && current_node->next)
	{
		if (ft_strcmp(current_node->key, current_node->next->key) > 0)
		{
			tmp = current_node->key;
			current_node->key = current_node->next->key;
			current_node->next->key = tmp;
			tmp = current_node->value;
			current_node->value = current_node->next->value;
			current_node->next->value = tmp;
		}
		current_node = current_node->next;
	}
}

void	export_no_args(t_env **en)
{
	t_env	*new;
	t_env	*current;

	new = NULL;
	current = *en;
	while (current)
	{
		add_var(current->key, current->value, &new, current->exp);
		sort(&new);
		current = current->next;
	}
	export_print(new); //free after.
}