#include "../includes/minishell.h"

int		in(char *red, t_scmd *scmd)
{
	int fd;

	if (scmd->fd_infile != -10)
		close(scmd->fd_infile);
	fd = open(red,O_RDONLY);
	if (fd == -1)
	{
		printf("%s: no such file or directory\n",red);
		return (0);
	}
	scmd->fd_infile = fd;
	return (1);
}

int		out(char *red, t_scmd *scmd)
{
	int fd;

	if (scmd->fd_outfile != -10)
		close(scmd->fd_outfile);
	fd = open(red, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// printf("TRUNC fd %d of file %s\n", fd, red);
	scmd->fd_outfile = fd;
	return (0);
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

char	*trim_quotes(char *red)
{
	char *trim_red;
	int i;
	int old_len;

	old_len = ft_strlen(red);
	trim_red = malloc(sizeof(char) * old_len - 1);
	i = 0;
	int j = 1;
	while (i < old_len - 1)
	{
		trim_red[i] = red[j];
		i++;
		j++;
	}
	trim_red[old_len - 2] = '\0';
	free(red);
	return (trim_red);
}

int 	heredoc(char *red, t_scmd *scmd)
{
	char *new_red;
	int q;

	q = 0;
	if (red[0] == '\'' || red[0] == '"')
	{
		new_red = trim_quotes(red);
		q = 1;
	}
	else
		new_red = red;
	prompt_heredoc(new_red,scmd);
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
		printf("BEFORE:----> \n%s",scmd->heredoc);
		expand_vars(&t,0);
		scmd->heredoc = t[0];
		// while (*t)
			// free(*t++);
		free(t);
		printf("AFTER :----> \n%s",scmd->heredoc);
	}
	return (0);
}

int 	append(char *red, t_scmd *scmd)
{
	int fd;

	if (scmd->fd_outfile != -10)
		close(scmd->fd_outfile);
	fd = open(red, O_WRONLY | O_CREAT | O_APPEND, 0644);
	// printf("append fd %d of file %s\n", fd, red);
	scmd->fd_outfile = fd;
	return (0);
}
