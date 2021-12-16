/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 16:50:49 by obounri           #+#    #+#             */
/*   Updated: 2021/12/13 19:37:35 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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

# define PIPE -30
# define UNQSPACE -31
# define EXPAND -32

static char impld[7][7] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};

typedef struct s_options
{
	struct  s_cmd *cmd;
	char    *user;
	char    *input;
	int     status;
	char    *curr_dir;
	char    **env;
	char    **path;
	char	*home;
	int		uncqu;
}   t_options;

typedef struct s_cmd
{
	int n_scmds;
	struct s_scmd *scmds;
}   t_cmd;

typedef struct s_scmd
{
	char *name;
	char *exec_path;
	char **args;
	int   impld;
	int	fd_infile;
	int fd_outfile;
}   t_scmd;

typedef	struct s_quote
{
	int	i;
	int	on;
	int dq;
	struct s_quote	*next;
}	t_quote;


int		ft_strcmp(char *s1, char *s2);
int		is_impld(char *name);
void	exec_impld(t_scmd	*scmd, t_options *opts);
t_quote *check_quotes_pipes(t_options	*opts);

#endif



//if !p
	// v 2
// 
// if p , if v 1 err
	// v 1



