#include "../includes/minishell.h"

int		in(char *red, t_scmd *scmd)
{
	int fd;

	if (scmd->fd_infile != -10)
		close(scmd->fd_infile);
	fd = open(red,O_RDONLY);
	if (fd == -1)
	{
		printf("no such file or directory: %s",red);
		return (0);
	}
	scmd->fd_infile = fd;
	return (0);
}

int		out(char *red, t_scmd *scmd)
{
	int fd;

	if (scmd->fd_outfile != -10)
		close(scmd->fd_outfile);
	fd = open(red, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	scmd->fd_outfile = fd;
	return (0);
}

int 	heredoc(char *red, t_scmd *scmd)
{
	char *heredoc;
	char *tmp;

	heredoc = "";
	tmp = NULL;
// Expand $VARs if "DEL" is quoted.
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

int 	append(char *red, t_scmd *scmd)
{
	int fd;

	if (scmd->fd_outfile != -10)
		close(scmd->fd_outfile);
	fd = open(red, O_WRONLY | O_CREAT | O_APPEND, 0644);
	scmd->fd_outfile = fd;
	return (0);
}
