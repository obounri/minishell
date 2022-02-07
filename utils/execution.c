/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 18:06:27 by obounri           #+#    #+#             */
/*   Updated: 2022/01/16 19:32:02 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	the_process(int in, int out, t_options *opts, int i)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (out != 1)
		dup2(out, 1);
	if (in != 0)
		dup2(in, 0);
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
			the_process(in, out, opts, i);
		waitpid(pid, &opts->status, 0);
		if (WIFSIGNALED(opts->status))
		{
			printf("\n");
			break ;
		}
		close(fd[1]);
		in = fd[0];
	}
}
