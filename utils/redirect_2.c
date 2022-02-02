/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:42:54 by obounri           #+#    #+#             */
/*   Updated: 2022/01/30 19:21:52 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_char_array(char *p)
{
	free(p);
	p = NULL;
}

int	in(char *red, t_scmd *scmd)
{
	int			fd;
	struct stat	buffer;

	if (scmd->fd_infile != -10)
		close(scmd->fd_infile);
	if (scmd->heredoc)
		free_char_array(scmd->heredoc);
	if (stat(red, &buffer) != -1)
	{
		if ((buffer.st_mode & S_IRUSR) == 0)
		{
			ft_error("minishell: ", red, ": Permission denied");
			return (0);
		}
	}
	fd = open(red, O_RDONLY);
	if (fd == -1)
	{
		ft_error("minishell: ", red, ": no such file or directory");
		return (0);
	}
	scmd->fd_infile = fd;
	return (1);
}

int	out(char *red, t_scmd *scmd)
{
	int			fd;
	struct stat	buffer;

	if (scmd->fd_outfile != -10)
		close(scmd->fd_outfile);
	if (stat(red, &buffer) != -1)
	{
		if ((buffer.st_mode & S_IWUSR) == 0)
		{
			ft_error("minishell: ", red, ": Permission denied");
			return (0);
		}
	}
	fd = open(red, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	scmd->fd_outfile = fd;
	return (1);
}

int	append(char *red, t_scmd *scmd)
{
	int			fd;
	struct stat	buffer;

	if (scmd->fd_outfile != -10)
		close(scmd->fd_outfile);
	if (stat(red, &buffer) != -1)
	{
		if ((buffer.st_mode & S_IWUSR) == 0)
		{
			ft_error("minishell: ", red, ": Permission denied");
			return (0);
		}
	}
	fd = open(red, O_WRONLY | O_CREAT | O_APPEND, 0644);
	scmd->fd_outfile = fd;
	return (1);
}
