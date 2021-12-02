#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include "../utils/libft/libft.h"
# include <sys/wait.h>
# include <dirent.h>


typedef struct s_options
{
	struct  s_cmd *cmd;
	char    *user;
	char    *input;
	int     status;
	char    *curr_dir;
	char    **env;
	char    **path;
}   t_options;

typedef struct s_cmd
{
	int n_scmds;
	struct s_scmd *scmds;
}   t_cmd;

typedef struct s_scmd
{
	char *exec_path;
	char **args;
	int   impld;
}   t_scmd;

int		ft_strcmp(char *s1, char *s2);
void	get_path(t_options	*opts);

#endif