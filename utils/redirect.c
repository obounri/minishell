#include "../includes/minishell.h"

	// if (<*n && arg) = arg | else if (< && <) = last_infile
	// if (> & >> & >) = last_outfile
	// if (| && < ) = infile
	// if (| && << ) = readline / delimiter

int		in(char *red, t_scmd *scmd)
{
	int fd;
	printf("--- Infile :%s\n",red);
// close prev fd, open new one.
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

	fd = open(red, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	printf("--- Outfile :%s\n",red);
	scmd->fd_outfile = fd;
	return (0);
}

int 	heredoc(char *red, t_scmd *scmd)
{
	char *heredoc = "";
	char *tmp;

	heredoc = "";
	tmp = NULL;
	printf("HEREDOC ON DELIMITER : %s\n",red);
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
	// printf("HEREDOC : \n%s",scmd->heredoc);
	return (0);
}

int 	append(char *red, t_scmd *scmd)
{
	printf("APPEND ON\n");
	int fd;

	fd = open(red, O_WRONLY | O_CREAT | O_APPEND, 0644);
	printf("--- APPENDfile :%s\n",red);
	scmd->fd_outfile = fd;
	return (0);
}

int		redirect_type(char *red, t_scmd *scmd, int type)
{
	if (type == IN)
		in(red,scmd);
	else if (type == OUT)
		out(red,scmd);
	else if (type == HEREDOC)
		heredoc(red,scmd);
	else if (type == APPEND)
		append(red,scmd);
	return (0);
}

int		redirect(char ***scmd, t_scmd *cmd,int type)
{
	char *red;
	char **tmp_cmd;
	int i;
	int infile;

	tmp_cmd = *scmd;
	i = -1;
	infile = 0;
	red = NULL;
	while (tmp_cmd[++i])
	{
		if (infile)
		{
			redirect_type(tmp_cmd[i],cmd,type);
			tmp_cmd[i] = ft_strdup("");
		}
		infile = 0;
		red = ft_strchr(tmp_cmd[i],type);
		if (red)
		{
			if (ft_strlen(red) != 1)
				redirect_type(red + 1,cmd,type);
			else
				infile = 1;
			if (red == tmp_cmd[i])
				tmp_cmd[i] = ft_strdup("");
			else
				tmp_cmd[i][red - tmp_cmd[i]] = '\0';
		}
	}
	if (infile)
		printf("Error, no infile provided");
	return (0);
}

int		cmp(char c, int token)
{
	if (c == (char)token)
		return (1);
	return (0);
}

int		check_errors(t_options *opts)
{
	check_pipe_errors(opts);
	check_in_errors(opts);
	check_out_errors(opts);
	return (1);
}

int		check_out_errors(t_options *opts)
{
	int i;
	int j;
	int out_counter;

	i = 0;
	j = 0;
	while (opts->input[i])
	{
		if (cmp(opts->input[i],OUT))
		{
			out_counter = 1;
			j = i;
			while (cmp(opts->input[++j],OUT))
				out_counter++;
			if (out_counter > 2)
				printf("TOOOOO MANY IN > REDIRECTIONS !!");
			else if(out_counter == 2)
			{
				opts->input[i] = APPEND;
				opts->input[i + 1] = UNQSPACE;
			}
			while (cmp(opts->input[j],UNQSPACE))
				j++;
			if (cmp(opts->input[j],OUT) && j != i + 1)
				printf("DUP");
			else if ((cmp(opts->input[j],PIPE) || cmp(opts->input[j],IN)))
				printf("Consecutive tokens");
		}
		i++;
	}
	return (0);
}

int		check_in_errors(t_options *opts)
{
	int i;
	int j;
	int in_counter;

	i = 0;
	j = 0;
	while (opts->input[i])
	{
		if (cmp(opts->input[i],IN))
		{
			in_counter = 1;
			j = i;
			while (cmp(opts->input[++j],IN))
				in_counter++;
			if (in_counter > 2)
				printf("TOOOOO MANY IN < REDIRECTIONS !!");
			else if (in_counter == 2)
			{
				opts->input[i] = HEREDOC;
				opts->input[i + 1] = UNQSPACE;
			}
			while (cmp(opts->input[j],UNQSPACE))
				j++;
			if (cmp(opts->input[j],IN) && j != i + 1)
				printf("DUP");
			else if ((cmp(opts->input[j],PIPE) || cmp(opts->input[j],OUT)))
				printf("Consecutive tokens");
		}
		i++;
	}
	return (0);
}

int		check_pipe_errors(t_options *opts)
{
	int len = ft_strlen(opts->input);
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < len)
	{
		if (cmp(opts->input[i],PIPE))
		{
			j = i;
			while (cmp(opts->input[++j],UNQSPACE));
			if (cmp(opts->input[j],PIPE) && j != i)
				printf("Duplicate PIPE");
			else if ((cmp(opts->input[j],IN) || cmp(opts->input[j],OUT)))
				printf("Consecutive tokens");
		}
		i++;
	}
	if (cmp(opts->input[0],PIPE) || cmp(opts->input[len - 1],PIPE)
		|| cmp(opts->input[len - 1],IN)
		|| cmp(opts->input[len - 1],OUT))
		{
			printf("ERRORRR");
			return (0);
		}
	return (1);
}