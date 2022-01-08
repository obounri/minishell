/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 18:58:49 by obounri           #+#    #+#             */
/*   Updated: 2022/01/08 14:16:52 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    unset_var(t_env **env, char *arg)
{
    t_env   *tmp_env;
    t_env   *prev;

    tmp_env = *env;
    while (tmp_env)
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

void    unset(t_options *opts, char **args, int child)
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
            opts->status = 1;
            continue ;
        }
        unset_var(&opts->env, args[i]);
    }
    if (child)
        exit(EXIT_SUCCESS);
}

void    ft_exit(char **args, int *status)
{
    int i;

    i = 0;

    while (args[0] && ft_isdigit(args[0][i]))
        i++;
    if (args[0] && args[0][i])
    {
        ft_putstr_fd("minishell: exit: fdv: numeric argument required\n", 2);
        exit(255);
    }
    i = 0;
    while (args[i])
        i++;
    if (i == 0)
        exit(0);
    else if (i > 1)
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        *status = 512;
    }
    else
        exit(ft_atoi(args[0]));
}