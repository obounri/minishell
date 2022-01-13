/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 17:35:32 by obounri           #+#    #+#             */
/*   Updated: 2022/01/13 12:37:58 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void    prompt(t_options *opts)
{
	if (WEXITSTATUS(opts->status) > 0 || WIFSIGNALED(opts->status))
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
	(void)sig;
	printf("\n");
	rl_on_new_line ();
	rl_replace_line("", 0);
	rl_redisplay();
}

char	*find_exec_path(t_options	*opts, char *name)
{
	int i;
	DIR *dp;
	struct dirent *dirp;
	char *var;

	var = ft_getenv(opts->env, "PATH");
	if (!var)
		return (NULL);
	opts->path = ft_split(var, ':');
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

int	parse_scmds(t_options	*opts, char **scmds)
{
	int i;
	int h; //	
	char **split_scmd;

	opts->cmd->scmds = malloc(sizeof(t_scmd) * (opts->cmd->n_scmds));
	init_scmds(opts->cmd->scmds, opts->cmd->n_scmds);
	i = -1;
	while (++i < opts->cmd->n_scmds)
	{
		split_scmd = ft_split(scmds[i], UNQSPACE);
		if (!split_scmd) // ??
			break; // ??
		expand_vars(&split_scmd, opts->env, opts->status);
		if (!redirect(&split_scmd, &opts->cmd->scmds[i], opts->env))
		{
			opts->cmd->scmds[i].err = 1;
			continue ;
		}
		new_alloc(&split_scmd);
		if (!split_scmd) // ??
			break; // ??
		h = -1;
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
	}
	return (1);
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
	if (!parse_scmds(opts, scmds))
		return (0);
	return (1);
}

int the_process(int in, int out, t_options *opts, int i, char **env)
{
	pid_t pid;

	pid = fork();
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
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
		else if (opts->cmd->scmds[i].err)
			exit(EXIT_FAILURE);
		else if (execve(opts->cmd->scmds[i].exec_path, opts->cmd->scmds[i].args, env) < 0)
		{
			ft_error("minishell: ", opts->cmd->scmds[i].args[0], ": command not found");
			exit(127);
		}
	}
	else
		waitpid(pid, &opts->status, 0);
	if (WIFSIGNALED(opts->status))
	{
		printf("\n");
		return (0);
	}
	return (1);
}

// print exit when ctrl-D ?
int main(int ac,char ** av, char **env)
{
	t_options	opts;
	int i, fd[2], in, out;

	(void)ac;
	(void)av;
	init(&opts, env);
	signal(SIGINT, &catch);
	while (1)
	{
		signal(SIGINT, &catch);
		rl_on_new_line ();
		opts.cmd->n_scmds = 1;
		if (opts.cmd->scmds)
			free(opts.cmd->scmds);
		opts.cmd->scmds  = NULL;
		prompt(&opts);
		if (opts.input == NULL)
			exit(0);
		if (parse_input(&opts) == 0)
			continue ;
		if (opts.cmd->n_scmds == 1 && opts.cmd->scmds[0].impld > 3)
		{
			exec_impld(&opts.cmd->scmds[0], &opts, 0);
			continue ;
		}
		i = 0;
		in = 0;
		while (i < opts.cmd->n_scmds)
		{
			pipe(fd);
			out = fd[1];
			if (opts.cmd->scmds[i].fd_infile != -10)
				in = opts.cmd->scmds[i].fd_infile;
			if (opts.cmd->scmds[i].fd_outfile != -10)
				out = opts.cmd->scmds[i].fd_outfile;
			else if (i == opts.cmd->n_scmds - 1)
				out = 1;
			if (!the_process(in, out, &opts, i, env))
				break ;
			close(fd[1]);
			in = fd[0];
			i++;
		}
	}
	return (0);
}