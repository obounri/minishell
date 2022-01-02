/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 17:35:32 by obounri           #+#    #+#             */
/*   Updated: 2021/12/25 11:45:09 by obounri          ###   ########.fr       */
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
	char **tmp;

	opts->cmd->scmds = malloc(sizeof(t_scmd) * (opts->cmd->n_scmds));
	i = 0;
	while (i < opts->cmd->n_scmds)
	{
		split_scmd = ft_split(scmds[i], UNQSPACE);
		if (!split_scmd)
			break;
		expand_vars(&split_scmd, opts->status);
		init_red(opts, split_scmd, &i);
		h = -1; 
		// exit(0);
		while (split_scmd && split_scmd[++h])
			if (split_scmd[h][0] == '"' || split_scmd[h][0] == '\'')
				split_scmd[h] = trim_quotes(split_scmd[h]);
		opts->cmd->scmds[i].impld = is_impld(split_scmd[0]);
		if (opts->cmd->scmds[i].impld < 0)
		{
			if (open(split_scmd[0], O_RDONLY) != -1)
				opts->cmd->scmds[i].exec_path = split_scmd[0];
			else
				opts->cmd->scmds[i].exec_path = find_exec_path(opts, split_scmd[0]);
		}
		opts->cmd->scmds[i].name = split_scmd[0];
		opts->cmd->scmds[i].args = &split_scmd[0];
		// h = 0; //
		// while (split_scmd[h]) //
		// 	printf("[%s]", split_scmd[h++]); //
		// printf("\n"); //
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
	if (!check_errors(opts))
		return (0);
	scmds = ft_split(opts->input, PIPE);
	if (check_scmds(scmds) == 0)
		return (0);
	while (scmds[opts->cmd->n_scmds])
		opts->cmd->n_scmds++;
	// int h = 0; //
	// while (scmds[h]) //
	// 	printf("[%s]", scmds[h++]); //
	// printf("\n"); //
	parse_scmds(opts, scmds);
	return (1);
}

// redirect input and output from and to infiles and outfiles here
int the_process(int in, int out, t_options *opts, int i, char **env)
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		if (out != 1)
		{
			dup2(out, 1);
			close(out);
		}
		if (in != 0)
		{
			dup2(in, 0);
			close(in);
		}
		if (opts->cmd->scmds[i].impld >= 0)
			exec_impld(&opts->cmd->scmds[i], opts, 1);
		else (execve(opts->cmd->scmds[i].exec_path, opts->cmd->scmds[i].args, env) < 0);
		{
			perror("minishell: command not found");
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, &opts->status, 0);
}

// execute this cmd : "ls -la | grep main | wc -l"
// trim args from "" and ''
// set status to success or failure in builtins
int main(int ac,char ** av, char **env)
{
	t_options	opts;
	pid_t		pid;
	int i = 0, fd[2], in = 0;

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
		i = 0;
		if (opts.cmd->n_scmds == 1 && opts.cmd->scmds[i].impld > 4)
		{
			exec_impld(&opts.cmd->scmds[i], &opts, 0);
			continue ;
		}
		while (i < opts.cmd->n_scmds)
		{
			// printf("\n----- name = %s, impld = %d, exec_path = %s -----\n", opts.cmd->scmds[i].name, opts.cmd->scmds[i].impld, opts.cmd->scmds[i].exec_path);
			// signal(SIGINT, SIG_DFL);
			pipe(fd);
			if (i == opts.cmd->n_scmds - 1)
				the_process(in, 1, &opts, i, env);	
			else
				the_process(in, fd[1], &opts, i, env);	
			close(fd[1]);
			in = fd[0];
			i++;
		}
	}
	return (0);
}
