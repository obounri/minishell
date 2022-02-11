/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 18:06:27 by obounri           #+#    #+#             */
/*   Updated: 2022/02/11 14:33:19 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	the_process(int in, int out, t_options *opts, int i, int fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fd);
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
	else if (execve(opts->cmd->scmds[i].exec_path,
			opts->cmd->scmds[i].args, opts->sh_env) < 0)
	{
		ft_error("minishell: ", opts->cmd->scmds[i].args[0],
			": command not found");
		exit(127);
	}
}

int	assign_in_out_fork(t_options *opts, int i, int *in, int *out)
{
	int		fd_hd[2];
	pid_t	pid;

	if (opts->cmd->scmds[i].heredoc)
	{
		pipe(fd_hd);
		write(fd_hd[1], opts->cmd->scmds[i].heredoc,
			ft_strlen(opts->cmd->scmds[i].heredoc));
		*in = fd_hd[0];
		close(fd_hd[1]);
	}
	else if (opts->cmd->scmds[i].fd_infile != -10)
		*in = opts->cmd->scmds[i].fd_infile;
	if (opts->cmd->scmds[i].fd_outfile != -10)
		*out = opts->cmd->scmds[i].fd_outfile;
	else if (i == (opts->cmd->n_scmds - 1))
		*out = 1;
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid != 0 && i == (opts->cmd->n_scmds - 1))
		opts->pids[0] = pid;
	return (pid);
}

void	exec(t_options *opts)
{
	int		i;
	int		fd[2];
	int		in;
	int		out;
	pid_t	pid;

	i = -1;
	in = 0;
	while (++i < opts->cmd->n_scmds)
	{
		pipe(fd);
		out = fd[1];
		pid = assign_in_out_fork(opts, i, &in, &out);
		if (pid == 0)
			the_process(in, out, opts, i, fd[0]);
		if (in != 0)
			close(in);
		if (out != 1)
			close(out);
		close(fd[1]);
		in = fd[0];
	}
	while ((pid = waitpid(-1, &opts->status, 0)) != -1)
	{
		if (pid == opts->pids[0])	
			opts->pids[1] = opts->status;
		if (pid != -1 && WIFSIGNALED(opts->status) && (WTERMSIG(opts->status) == 2 || WTERMSIG(opts->status) == 3))
		{
			printf("\n");
			pid = -1;
		}
	}
	opts->status = opts->pids[1];
}
	