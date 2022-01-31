/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 17:35:32 by obounri           #+#    #+#             */
/*   Updated: 2022/01/19 11:25:49 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	prompt(t_options *opts)
{
	char	*tmp;

	if (WEXITSTATUS(opts->status) > 0 || WIFSIGNALED(opts->status))
		opts->prompt = ft_strdup("\033[0;31m");
	else
		opts->prompt = ft_strdup("\033[0;32m");
	tmp = ft_strdup(opts->prompt);
	free(opts->prompt);
	opts->prompt = ft_strjoin(tmp, "➤ \033[0m");
	free(tmp);
	tmp = ft_strdup(opts->prompt);
	free(opts->prompt);
	opts->prompt = ft_strjoin(tmp, opts->curr_dir);
	free(tmp);
	tmp = ft_strdup(opts->prompt);
	free(opts->prompt);
	opts->prompt = ft_strjoin(tmp, " ~ ");
	free(tmp);
	if (opts->input)
		free(opts->input);
	opts->input = readline(opts->prompt);
	free(opts->prompt);
	opts->prompt = NULL;
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
		signal(SIGINT, &catch);
		signal(SIGQUIT, SIG_IGN);
		prompt(&opts);
		if (opts.input == NULL)
			clean_exit(&opts, 0);
		opts.cmd->n_scmds = 1;
		if (parse_input(&opts) == 0)
			continue ;
		if (opts.cmd->n_scmds == 1 && opts.cmd->scmds[0].impld > 2)
			exec_impld(&opts.cmd->scmds[0], &opts, 0);
		else
			exec(&opts);
		free_scmds(&opts.cmd->scmds, opts.cmd->n_scmds);
	}
	return (0);
}
