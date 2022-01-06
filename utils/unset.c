/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 18:58:49 by obounri           #+#    #+#             */
/*   Updated: 2022/01/06 19:46:43 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    unset_var(t_env **env, char *arg)
{
    t_env   *tmp_env;
    t_env   *prev;

    tmp_env = *env;
    while (tmp_env->next)
    {
        if (ft_strcmp(tmp_env->key, arg) == 0)
        {
            prev->next = tmp_env->next;
            free(tmp_env);
            return ;
        }
        prev = tmp_env;
        tmp_env = tmp_env->next;
    }   
}

void    unset(t_env **env, char **args, int *status, int child)
{
    int i;
    int j;

    i = -1;
    while(args[++i])
    {
        j = 0;
        while (args[i][j] && (ft_isalnum(args[i][j]) == 1 || args[i][j] == '_'))
            j++;
        if (args[i][j])
        {
            ft_putstr_fd("minishell: unset: `", 2);
            ft_putstr_fd(args[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            *status = 1;
            continue ;
        }
        unset_var(env, args[i]);
    }
    if (child)
        exit(EXIT_SUCCESS);
}