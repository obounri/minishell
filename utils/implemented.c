/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implemented.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 16:50:43 by obounri           #+#    #+#             */
/*   Updated: 2022/02/07 15:49:34 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_impld(char *name)
{
	int			i;
	static char	impld[7][7] = {"echo", "env",
		"pwd", "export", "unset", "cd", "exit"};

	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(name, impld[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

void	echo(char **args)
{
	int	i;
	int	j;
	int	opt;

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

void	cd(char **args, t_options	*opts, int child)
{
	char	*tmp;

	if (!args[1])
		chdir(opts->home);
	else if (chdir(args[1]) < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(args[1]);
		opts->status = 512;
		if (child)
			exit(2);
		return ;
	}
	tmp = getcwd(NULL, 0);
	if (!tmp)
		ft_error("cd: error retrieving current directory:",
			NULL, "getcwd: cannot access parent \
			directories: No such file or directory");
	else
		opts->curr_dir = ft_strdup(tmp);
	free(tmp);
	opts->status = 0;
	if (child)
		exit(EXIT_SUCCESS);
}

void	pwd(char *path)
{
	printf("%s\n", path);
	exit(EXIT_SUCCESS);
}

void	env(t_env *env)
{
	while (env)
	{
		if (!env->exp)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	exit(EXIT_SUCCESS);
}
