/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 17:35:32 by obounri           #+#    #+#             */
/*   Updated: 2021/11/29 17:21:30 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void    prompt(t_options *opts)
{
	if (WEXITSTATUS(opts->status) > 0)
		printf("\033[0;31m");
	else
		printf("\033[0;32m");
	printf("➤ ");
	printf("\033[0m");
	printf("%s / %s ~ ", opts->curr_dir, opts->user);
	opts->input = readline("");
}

void		catch(int sig)
{
	printf("%d\n", sig);
	return ;
}

int	ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

char	*find_exec_path(t_options	*opts, char *name)
{
	// free splited before return
	char **var;
	int i = 0;
	char **path;
	DIR *dp;
    struct dirent *dirp;

	// add path variable to t_options and initialize it like so :
	while (opts->env[i])
	{
		var = ft_split(opts->env[i], '=');
		if (ft_strcmp("PATH", var[0]) == 0)
		{
			path = ft_split(var[1], ':');
			break ;
		}
		i++;
	}
	i = -1;
	while (path[++i])
	{
		dp = opendir(path[i]);
		if (!dp)
			continue ;
		while ((dirp = readdir(dp)) != NULL)
		{
			if (ft_strcmp(dirp->d_name, name) == 0)
				return (ft_strjoin(ft_strjoin(path[i], "/"), name));
		}
		closedir(dp);
	}
	return (NULL);
}

int	parse(t_options	*opts)
{
	int i = 0;
	char **splited;

	splited = ft_split(opts->input, ' ');
	if (!splited[0])
		return (0);
	while (splited[i])
		i++;
	opts->cmd->n_scmds = i;
	// opts->cmd->scmds = malloc(sizeof(t_scmd) * (i + 1));
	opts->cmd->scmds = malloc(sizeof(t_scmd) * (i + 1));
	opts->cmd->scmds[0].impld = 0;
	opts->cmd->scmds[0].exec_path = find_exec_path(opts, splited[0]);
	opts->cmd->scmds[0].args = &splited[1];
	return (1);
}

int main(int ac,char ** av, char **env)
{
	t_options	opts;
	pid_t pid;


	opts.user = readline("Enter user name for prompt: ");
	opts.status = 0;
	opts.curr_dir = getcwd(NULL, 0);
	opts.env = env;
	opts.cmd = malloc(sizeof(t_cmd));
	while (1)
	{
		opts.cmd->scmds  = NULL;
		signal(SIGINT, &catch);
		prompt(&opts);
		if (opts.input == NULL)
			exit(0) ;
		if (parse(&opts) == 0)
			continue ;
		
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			if (execve(opts.cmd->scmds[0].exec_path, opts.cmd->scmds[0].args, env) < 0)
			{
				perror("fsh: command not found");
				exit(1);
			}
		}
		else
			waitpid(pid, &opts.status, 0);
		free(opts.cmd->scmds);
	}
	return (0);
}