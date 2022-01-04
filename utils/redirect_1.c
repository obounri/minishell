#include "../includes/minishell.h"

int		init_red(t_options *opts, char **split_scmd, int *i, int *order)
{
	int j;

	j = -1;
	opts->cmd->scmds[*i].fd_infile= -10;
	opts->cmd->scmds[*i].fd_outfile= -10;
	while (order[++j])
	{
		// printf("token %d = %d\n", j, order[j]);
		if (order[j] == IN)
		{
			if (!redirect(&split_scmd,&opts->cmd->scmds[*i],IN))
				return (0);
		}
		else if (order[j] == HEREDOC)
			redirect(&split_scmd,&opts->cmd->scmds[*i],HEREDOC);
		else if (order[j] == APPEND)
			redirect(&split_scmd,&opts->cmd->scmds[*i],APPEND);
		else if (order[j] == OUT)
			redirect(&split_scmd,&opts->cmd->scmds[*i],OUT);
	}
	return (1);
}

int		redirect_type(char *red, t_scmd *scmd, int type)
{
	printf("red %s\n", red);
	if (type == IN)
	{
		if (!in(red,scmd))
			return (0);
	}
	else if (type == OUT)
		out(red,scmd);
	else if (type == HEREDOC)
		heredoc(red,scmd);
	else if (type == APPEND)
		append(red,scmd);
	return (1);
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

	tmp_cmd = *scmd;
	i = -1;
	infile = 0;
	red = NULL;
	while (tmp_cmd[++i])
	{
		if (infile)
		{
			if (!redirect_type(tmp_cmd[i],cmd,type))
				return (0);
			tmp_cmd[i] = ft_strdup("");
			return (1);
		}
		infile = 0;
		red = ft_strchr(tmp_cmd[i],type);
		if (red)
		{
			if (ft_strlen(red) != 1)
			{
				if (!redirect_type(red + 1,cmd,type))
					return (0);
				tmp_cmd[i] = ft_strdup("");
				return (1);
			}
			else
				infile = 1;
			if (red == tmp_cmd[i])
				tmp_cmd[i] = ft_strdup("");
			else
				tmp_cmd[i][red - tmp_cmd[i]] = '\0';
		}
	}
	return (1);
}