/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 16:50:49 by obounri           #+#    #+#             */
/*   Updated: 2022/01/13 11:14:26 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include "../utils/libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <fcntl.h>
# include <errno.h>

# define PIPE -30
# define UNQSPACE -31
# define EXPAND -32
# define IN -33
# define OUT -34
# define APPEND -35
# define HEREDOC -36

static char impld[7][7] = {"echo", "env", "pwd", "export", "unset", "cd", "exit"};

typedef struct s_options
{
	struct  s_cmd *cmd;
	struct	s_env *env;
	char    *user;
	char	*prompt;
	char    *input;
	int     status;
	char    *curr_dir;
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
	char *heredoc;
	int	fd_infile;
	int fd_outfile;
	int err;
}   t_scmd;

typedef	struct s_quote
{
	int	i;
	int	on;
	int dq;
	struct s_quote	*next;
}	t_quote;

typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env *next;
}	t_env;

int		ft_strcmp(char *s1, char *s2);
int		is_impld(char *name);
void	exec_impld(t_scmd	*scmd, t_options *opts, int child);
t_quote	*check_quotes_pipes(t_options	*opts);
void	expand_vars(char ***scmd, t_env *env, int status);
char	*expand(char **scmd, int j, t_env *env, int status);
void	init(t_options *opts, char **env);
char	*ft_getenv(t_env *env, char *key);
void	init_scmds(t_scmd *scmds, int n_scmds);
void    unset(t_options *opts, char **args, int child);
void    ft_exit(char **args, int *status);

//Redirection
// int		init_red(t_options *opts, char **split_scmd, int *i, int *order);
int		redirect_type(char *red, t_scmd *scmd, int type, t_env *env);
int		redirect(char ***scmd, t_scmd *cmd, t_env *env);
int		in(char *red, t_scmd *scmd);
int		out(char *red, t_scmd *scmd);
int 	heredoc(char *red, t_scmd *scmd, t_env *env);
int 	append(char *red, t_scmd *scmd);
int		new_alloc_size(char **cmd);
void	new_alloc(char ***cmd);

//Error checking
int		cmp(char c, int token);
int		check_errors(t_options *opts);
int		check_out_errors(t_options *opts);
int		check_in_errors(t_options *opts);
int		check_pipe_errors(t_options *opts);
int		error_msg();
int		check_scmds(char **scmds);
int		is_empty(char *input);
void	ft_error(char *prob, char *var, char *err);

char	*trim_quotes(char *red);

#endif