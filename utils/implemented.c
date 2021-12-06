/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implemented.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 16:50:43 by obounri           #+#    #+#             */
/*   Updated: 2021/12/06 22:40:48 by obounri          ###   ########.fr       */
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
	int	i;
	int opt;

	i = 1;
	opt = 0;
	if (args[1] && (ft_strcmp(args[1], "-n") == 0))
	{
		i = 2;
		opt = 1;
	}
	while (args[i])
		ft_putstr_fd(args[i++], 1);
	if (!opt)
		ft_putchar_fd('\n', 1);
}

void	cd(char **args, t_options	*opts)
{
	if (!args[1])
		chdir(opts->home);
	else if (chdir(args[1]) < 0)
		perror(args[1]);
	opts->curr_dir = getcwd(NULL, 0);
}

void	env(char **env)
{
	int i;

	i = 0;
	while (env[i] != NULL)
		printf("%s\n", env[i++]);
}

void	exec_impld(t_scmd	*scmd, t_options	*opts)
{
	if (ft_strcmp(scmd->name, "echo") == 0)
		echo(scmd->args);
	else if (ft_strcmp(scmd->name, "cd") == 0)
		cd(scmd->args, opts);
	else if (ft_strcmp(scmd->name, "pwd") == 0)
		printf("%s\n", opts->curr_dir);
	else if (ft_strcmp(scmd->name, "export") == 0)
		printf("export\n");
	else if (ft_strcmp(scmd->name, "unset") == 0)
		printf("unset\n");
	else if (ft_strcmp(scmd->name, "env") == 0)
		env(opts->env);
	else if (ft_strcmp(scmd->name, "exit") == 0)
		exit(0);
}
