/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 17:35:32 by obounri           #+#    #+#             */
/*   Updated: 2022/01/18 13:31:29 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	prompt(t_options *opts)
{
	opts->cmd->n_scmds = 1;
	if (opts->cmd->scmds)
		free(opts->cmd->scmds);
	opts->cmd->scmds = NULL;
	if (WEXITSTATUS(opts->status) > 0 || WIFSIGNALED(opts->status))
		opts->prompt = ft_strdup("\033[0;31m");
	else
		opts->prompt = ft_strdup("\033[0;32m");
	opts->prompt = ft_strjoin(opts->prompt, "➤ \033[0m");
	opts->prompt = ft_strjoin(opts->prompt, opts->curr_dir);
	opts->prompt = ft_strjoin(opts->prompt, " ~ "); // free
	opts->input = readline(opts->prompt); // free
}

void	catch(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line ();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int ac, char **av, char **env)
{
	t_options	opts;

	(void)ac;
	(void)av;
	init(&opts, env);
	while (1)
	{
		if (opts.cmd->scmds)
			free_scmds(&opts.cmd->scmds);
		signal(SIGINT, &catch);
		signal(SIGQUIT, SIG_IGN);
		prompt(&opts);
		if (opts.input == NULL)
			clean_exit(&opts, 0);
		if (parse_input(&opts) == 0)
			continue ;
		if (opts.cmd->n_scmds == 1 && opts.cmd->scmds[0].impld > 2)
			exec_impld(&opts.cmd->scmds[0], &opts, 0);
		else
			exec(&opts);
	}
	return (0);
}
