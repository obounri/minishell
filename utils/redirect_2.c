/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:42:54 by obounri           #+#    #+#             */
/*   Updated: 2022/01/13 16:56:42 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		in(char *red, t_scmd *scmd)
{
	int fd;
	struct stat buffer;

	if (scmd->fd_infile != -10)
		close(scmd->fd_infile);
	if (scmd->heredoc)
	{
		free(scmd->heredoc);
		scmd->heredoc = NULL;
	}
	if (stat(red, &buffer) != -1)
		if ((buffer.st_mode & S_IRUSR) == 0)
			{
				ft_error("minishell: ", red, ": Permission denied");
				return (0);
			}
	fd = open(red,O_RDONLY);
	if (fd == -1)
	{
		ft_error("minishell: ", red, ": no such file or directory");
		return (0);
	}
	scmd->fd_infile = fd;
	return (1);
}

int		out(char *red, t_scmd *scmd)
{
	int fd;
	struct stat buffer;

	if (scmd->fd_outfile != -10)
		close(scmd->fd_outfile);
	if (stat(red, &buffer) != -1)
		if ((buffer.st_mode & S_IWUSR) == 0)
		{
			ft_error("minishell: ", red, ": Permission denied");
			return (0);
		}
	fd = open(red, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	scmd->fd_outfile = fd;
	return (1);
}

int		prompt_heredoc(char *red, t_scmd *scmd)
{
	char *heredoc;
	char *tmp;

	heredoc = "";
	tmp = NULL;
	while (1)
	{
		tmp = readline(">");
		if (ft_strcmp(red,tmp) == 0)
			break;
		if (!tmp)
			heredoc = ft_strjoin(heredoc,"");
		else
			heredoc = ft_strjoin(ft_strjoin(heredoc,tmp),"\n");
	}
	scmd->heredoc = heredoc;
	return (0);
}

int 	heredoc(char *red, t_scmd *scmd, t_env *env)
{
	char *new_red;
	int q;

	if (scmd->fd_infile != -10)
		close(scmd->fd_infile);
	if (scmd->heredoc)
	{
		free(scmd->heredoc);
		scmd->heredoc = NULL;
	}
	q = 0;
	if (red[0] == '\'' || red[0] == '"')
	{
		new_red = trim_quotes(red);
		q = 1;
	}
	else
		new_red = red;
	prompt_heredoc(new_red, scmd);
	if (!q)
	{
		int i = -1;
		char **t;

		t = malloc(sizeof (char *) * 2);
		t[1] = NULL;
		while (scmd->heredoc[++i])
			if (scmd->heredoc[i] == '$')
				scmd->heredoc[i] = EXPAND;
		t[0] = scmd->heredoc;
		expand_vars(&t, env, 0);
		scmd->heredoc = ft_strdup(t[0]);
		free(t);
	}
	return (0);
}

int 	append(char *red, t_scmd *scmd)
{
	int fd;
	struct stat buffer;

	if (scmd->fd_outfile != -10)
		close(scmd->fd_outfile);
	if (stat(red, &buffer) != -1)
		if ((buffer.st_mode & S_IWUSR) == 0)
		{
			ft_error("minishell: ", red, ": Permission denied");
			return (0);
		}
	fd = open(red, O_WRONLY | O_CREAT | O_APPEND, 0644);
	scmd->fd_outfile = fd;
	return (1);
}
