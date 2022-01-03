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

int		new_alloc_size(char **cmd)
{
	int i;
	int new_size;

	i = 0;
	new_size = 0;
	while (cmd && cmd[i])
	{
		if (is_empty(cmd[i]))
			new_size++;
		i++;
	}
	return (new_size);
}

int		tab_len(char **tab)
{
	int i;

	i = 0;
	while (tab && tab[i])
		i++;
	return (i);
}

void	free_tab(char **tab)
{
	int i;

	i = 0;
	if (tab){
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	}
}

void	new_alloc(char ***cmd)
{
	int i;
	char **new;
	int size;

	i = -1;
	int j = -1;
	size = new_alloc_size(*cmd);
	new = malloc(sizeof(char *) * size + 1);
	while ((*cmd)[++i])
	{
		if (ft_strcmp((*cmd)[i],""))
			new[++j] = (*cmd)[i];
		else
			free((*cmd)[i]);
	}
	new[++j] = NULL;
	free(*cmd);
	*cmd = new;
}

int		redirect(char ***scmd, t_scmd *cmd, int type)
{
	char *red;
	char **tmp_cmd;
	int i;
	int infile;
	int d = 0;

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
	return (0);
}