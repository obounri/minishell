/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 17:35:32 by obounri           #+#    #+#             */
/*   Updated: 2021/12/13 19:42:33 by obounri          ###   ########.fr       */
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
	printf("sig $%d\n", sig);
	return ;
}

char	*find_exec_path(t_options	*opts, char *name)
{
	int i;
	DIR *dp;
	struct dirent *dirp;
	char *var; //

	// move to init
	var = getenv("PATH"); //
	opts->path = ft_split(var, ':'); //
	i = -1;
	while (opts->path[++i])
	{
		dp = opendir(opts->path[i]);
		if (!dp)
			continue ;
		while ((dirp = readdir(dp)) != NULL)
		{
			if (ft_strcmp(dirp->d_name, name) == 0)
				return (ft_strjoin(ft_strjoin(opts->path[i], "/"), name));
		}
		closedir(dp);
	}
	return (NULL);
}

// int	parse(t_options	*opts)
// {
// 	int i = 0;
// 	char **splited;

// 	splited = ft_split(opts->input, ' ');
// 	if (!splited[0])
// 		return (0);
// 	while (splited[i])
// 		i++;
// 	opts->cmd->n_scmds = i;
// 	// opts->cmd->scmds = malloc(sizeof(t_scmd) * (i + 1));
// 	opts->cmd->scmds = malloc(sizeof(t_scmd) * (i + 1));
// 	// opts->cmd->scmds[0].impld = 0;
// 	opts->cmd->scmds[0].impld = is_impld(splited[0]);
// 	if (opts->cmd->scmds[0].impld < 0)
// 		opts->cmd->scmds[0].exec_path = find_exec_path(opts, splited[0]);
// 	opts->cmd->scmds[0].name = splited[0];
// 	opts->cmd->scmds[0].args = &splited[0];
// 	return (1);
//}

int	parse_test(t_options	*opts)
{
	t_quote *quotes;
	int *split_here;
	char	**scmds = NULL;
	int i =0;
	if (!opts->input[0])
		return (0);
	quotes = NULL;
	quotes = check_quotes_pipes(opts);
	scmds = ft_split(opts->input,PIPE);
	while (scmds && scmds[i])
	{
		printf("%s\n",scmds[i]);
		i++;
	}

	while (quotes)
	{
		printf("index %d, on = %d, dq = %d\n", quotes->i, quotes->on, quotes->dq);
		quotes = quotes->next;
	}
	return (1);
}

int main(int ac,char ** av, char **env)
{
	t_options	opts;
	pid_t		pid;

	// if (<*n && arg) = arg | else if (< && <) = last_infile
	// if (> & >> & >) = last_outfile
	// if (| && < ) = infile
	// if (| && << ) = readline / delimiter

	opts.user = readline("Enter user name for prompt: ");
	opts.status = 0;
	opts.curr_dir = getcwd(NULL, 0);
	opts.home = getenv("HOME");
	opts.env = env;
	opts.cmd = malloc(sizeof(t_cmd));
	opts.cmd->scmds  = NULL;
	while (1)
	{
		// signal(SIGINT, &catch);
		if (opts.cmd->scmds)
			free(opts.cmd->scmds);
		opts.cmd->scmds  = NULL;
		prompt(&opts);
		if (opts.input == NULL)
			exit(0) ;
		if (parse_test(&opts) == 0)
			continue ;
		// if (opts.cmd->scmds[0].impld >= 0)
		// {
		// 	exec_impld(&opts.cmd->scmds[0], &opts);
		// 	continue ;
		// }
		// pid = fork();
		// if (pid == 0)
		// {
		// 	signal(SIGINT, SIG_DFL);
		// 	if (execve(opts.cmd->scmds[0].exec_path, opts.cmd->scmds[0].args, env) < 0)
		// 	{
		// 		perror("fsh: command not found");
		// 		exit(1);
		// 	}
		// }
		// else
		// 	waitpid(pid, &opts.status, 0);
	}
	return (0);
}