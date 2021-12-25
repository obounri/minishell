/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 17:35:32 by obounri           #+#    #+#             */
/*   Updated: 2021/12/24 15:16:58 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

// prompt disapearring and not prompting new line on linux problem was here,
// all prompt is given to readline now
void    prompt(t_options *opts)
{
	if (WEXITSTATUS(opts->status) > 0)
		opts->prompt = ft_strdup("\033[0;31m");
	else
		opts->prompt = ft_strdup("\033[0;32m");
	opts->prompt = ft_strjoin(opts->prompt, "➤ \033[0m");
	opts->prompt = ft_strjoin(opts->prompt, opts->curr_dir);
	opts->prompt = ft_strjoin(opts->prompt, " / ");
	opts->prompt = ft_strjoin(opts->prompt, opts->user);
	opts->prompt = ft_strjoin(opts->prompt, " ~ "); // free
	opts->input = readline(opts->prompt); // free
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

void	parse_scmds(t_options	*opts, char **scmds)
{
	int i;
	int h; //
	char **split_scmd;

	opts->cmd->scmds = malloc(sizeof(t_scmd) * (opts->cmd->n_scmds));
	i = 0;
	while (i < opts->cmd->n_scmds)
	{
		split_scmd = ft_split(scmds[i], UNQSPACE);
		expand_vars(&split_scmd, opts->status);
		redirect(&split_scmd,&opts->cmd->scmds[i],IN);
		redirect(&split_scmd,&opts->cmd->scmds[i],OUT);
		redirect(&split_scmd,&opts->cmd->scmds[i],HEREDOC);
		redirect(&split_scmd,&opts->cmd->scmds[i],APPEND);
		opts->cmd->scmds[i].impld = is_impld(split_scmd[0]);
		if (opts->cmd->scmds[i].impld < 0)
			opts->cmd->scmds[i].exec_path = find_exec_path(opts, split_scmd[0]);
		opts->cmd->scmds[i].name = split_scmd[0];
		opts->cmd->scmds[i].args = &split_scmd[0];
		h = 0; //
		while (split_scmd[h]) //
			printf("[%s]", split_scmd[h++]); //
		printf("\n"); //
		i++;
	}
	return ;
}

int	parse_input(t_options	*opts)
{
	t_quote *quotes;
	char	**scmds = NULL;
	
	if (!opts->input[0])
		return (0);
	add_history(opts->input);
	quotes = check_quotes_pipes(opts);
	if (opts->uncqu == 1)
	{
		opts->uncqu = 0;
		return (0);
	}
	scmds = ft_split(opts->input, PIPE);
	while (scmds[opts->cmd->n_scmds])
		opts->cmd->n_scmds++;
	// int h = 0; //
	// while (scmds[h]) //
	// 	printf("[%s]", scmds[h++]); //
	// printf("\n"); //
	parse_scmds(opts, scmds);
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

	opts.user = readline("Enter user name for prompt: "); // free
	opts.status = 0;
	opts.curr_dir = getcwd(NULL, 0);
	opts.home = getenv("HOME");
	opts.env = env;
	opts.prompt = NULL;
	opts.cmd = malloc(sizeof(t_cmd));
	opts.cmd->scmds  = NULL;
	opts.uncqu = 0;
	using_history();
	while (1)
	{
		// history is added before any modif in parse_input()
		// to be given to signal a new line to readline
		rl_on_new_line ();
		opts.cmd->n_scmds = 1;
		// signal(SIGINT, &catch);
		if (opts.cmd->scmds)
			free(opts.cmd->scmds);
		opts.cmd->scmds  = NULL;
		prompt(&opts);
		if (opts.input == NULL)
			exit(0);
		if (parse_input(&opts) == 0)
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
