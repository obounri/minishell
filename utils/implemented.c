/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implemented.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 16:50:43 by obounri           #+#    #+#             */
/*   Updated: 2022/01/05 15:59:19 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int		is_impld(char *name)
{
	int	i;

	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(name, impld[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

void    echo(char **args)
{
	int	i, j;
	int opt;

	i = 1;
	opt = 0;
	while (args[i] && (ft_strncmp(args[i], "-n", 2) == 0))
	{
		j = 2;
		while (args[i][j] && args[i][j] == 'n')
			j++;
		if (args[i][j])
			break ;
		opt = 1;
		i++;
	}
	while (args[i])
		printf("%s ", args[i++]);
	if (!opt)
		printf("\n");
	exit(EXIT_SUCCESS);
}

void	cd(char **args, t_options	*opts, int cd_exit)
{
	if (!args[1])
		chdir(opts->home);
	else if (chdir(args[1]) < 0)
	{
		perror(args[1]);
		opts->status = 256;
		if (cd_exit)
			exit(EXIT_FAILURE);
		return ;
	}
	opts->curr_dir = getcwd(NULL, 0);
	if (cd_exit)
		exit(EXIT_SUCCESS);
}

void	pwd(char *path)
{
	printf("%s\n", path);
	exit(EXIT_SUCCESS);
}

void	unset(void)
{
	printf("unset\n");
	exit(EXIT_SUCCESS);
}

void	env(t_env *env)
{
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	exit(EXIT_SUCCESS);
}

void	exec_impld(t_scmd	*scmd, t_options	*opts, int cd_exit)
{
	if (ft_strcmp(scmd->name, "echo") == 0)
		echo(scmd->args);
	else if (ft_strcmp(scmd->name, "cd") == 0)
		cd(scmd->args, opts, cd_exit);
	else if (ft_strcmp(scmd->name, "pwd") == 0)
		pwd(opts->curr_dir);
	else if (ft_strcmp(scmd->name, "export") == 0)
		export(scmd->args,opts->env);
	else if (ft_strcmp(scmd->name, "unset") == 0)
		unset();
	else if (ft_strcmp(scmd->name, "env") == 0)
		env(opts->env);
	else if (ft_strcmp(scmd->name, "exit") == 0)
		exit(0);
}
