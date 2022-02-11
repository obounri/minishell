/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 18:06:27 by obounri           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/02/11 18:28:52 by obounri          ###   ########.fr       */
=======
/*   Updated: 2022/02/11 16:13:16 by obounri          ###   ########.fr       */
>>>>>>> d2eb5b35b2e84337c45e468b11aa1d8f5b1f4e48
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	the_process(int in, int out, t_options *opts, int i)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
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
		opts->tmp[0] = pid;
	return (pid);
}

void	parent(t_options *opts)
{
	pid_t	pid;

	pid = waitpid(-1, &opts->status, 0);
	while (pid != -1)
	{
		if (pid == opts->tmp[0])
			opts->tmp[1] = opts->status;
		if (pid != -1 && WIFSIGNALED(opts->status)
			&& (WTERMSIG(opts->status) == 2 || WTERMSIG(opts->status) == 3))
		{
			printf("\n");
			pid = -1;
		}
		pid = waitpid(-1, &opts->status, 0);
	}
	opts->status = opts->tmp[1];
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
<<<<<<< HEAD
		if (pid == 0 && !close(fd[0]))
=======
		if ((pid == 0) && !close(fd[0]))
>>>>>>> d2eb5b35b2e84337c45e468b11aa1d8f5b1f4e48
			the_process(in, out, opts, i);
		if (in != 0)
			close(in);
		if (out != 1)
			close(out);
		close(fd[1]);
		in = fd[0];
	}
	parent(opts);
}
