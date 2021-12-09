/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 17:35:32 by obounri           #+#    #+#             */
/*   Updated: 2021/12/09 00:48:19 by obounri          ###   ########.fr       */
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
	// opts->cmd->scmds[0].impld = 0;
	opts->cmd->scmds[0].impld = is_impld(splited[0]);
	if (opts->cmd->scmds[0].impld < 0)
		opts->cmd->scmds[0].exec_path = find_exec_path(opts, splited[0]);
	opts->cmd->scmds[0].name = splited[0];
	opts->cmd->scmds[0].args = &splited[0];
	return (1);
}

// void	new_quote(t_quote *quotes, int i, int on, int dq)
// {
// 	t_quote *quote, *tmp;

// 	quote = malloc(sizeof(t_quote) * 1);
// 	quote->i = i;
// 	if (on >= 0)
// 		quote->on = 1;
// 	else
// 		quote->on = 0;
// 	quote->dq = dq;
// 	quote->next = NULL;
// 	if (!quotes)
// 		quotes = quote;
// 	else
// 	{
// 		while (quotes->next)
// 			quotes = quotes->next;
// 		quotes->next = quote;
// 	}
// }

// int	parse_test(t_options	*opts)
// {
// 	int i = 0, j = 0, dq = -1;
// 	t_quote *quotes;
// 	int *split_here;

// 	quotes = NULL;
// 	while(opts->input[i])
// 	{
// 		if (opts->input[i] == '"' && (dq != 0)) 
// 		{
// 			if (dq == 1)
// 				dq = -1;
// 			else if (dq == -1)
// 				dq == 1;
// 			new_quote(quotes, i, dq, 1);
// 		}
// 		else if (opts->input[i] == '\''  && (dq != 1))
// 		{
// 			if (dq == 0)
// 				dq = -1;
// 			else if (dq == -1)
// 				dq == 0;
// 			new_quote(quotes, i, dq, 0);
// 		}
// 	}
// 	return (1);
// }

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
		signal(SIGINT, &catch);
		if (opts.cmd->scmds)
			free(opts.cmd->scmds);
		opts.cmd->scmds  = NULL;
		prompt(&opts);
		if (opts.input == NULL)
			exit(0) ;
		if (parse(&opts) == 0)
			continue ;
		if (opts.cmd->scmds[0].impld >= 0)
		{
			exec_impld(&opts.cmd->scmds[0], &opts);
			continue ;
		}
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
	}
	return (0);
}