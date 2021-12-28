#include "../includes/minishell.h"

int		init_red(t_options *opts, char **split_scmd, int *i)
{
	opts->cmd->scmds[*i].fd_infile= -10;
	opts->cmd->scmds[*i].fd_outfile= -10;
	redirect(&split_scmd,&opts->cmd->scmds[*i],IN);
	redirect(&split_scmd,&opts->cmd->scmds[*i],OUT);
	redirect(&split_scmd,&opts->cmd->scmds[*i],HEREDOC);
	redirect(&split_scmd,&opts->cmd->scmds[*i],APPEND);
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