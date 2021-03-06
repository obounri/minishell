/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 16:50:49 by obounri           #+#    #+#             */
/*   Updated: 2022/02/11 19:42:35 by obounri          ###   ########.fr       */
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

# define SQ -28
# define DQ -29
# define PIPE -30
# define UNQSPACE -31
# define EXPAND -32
# define IN -33
# define OUT -34
# define APPEND -35
# define HEREDOC -36

typedef struct s_options
{
	struct s_cmd	*cmd;
	struct s_env	*env;
	char			*prompt;
	char			*input;
	int				status;
	char			*curr_dir;
	char			**path;
	char			**sh_env;
	char			*home;
	int				uncqu;
	int				tmp[2];
}	t_options;

typedef struct s_cmd
{
	int				n_scmds;
	struct s_scmd	*scmds;
}	t_cmd;

typedef struct s_scmd
{
	char	**scmd;
	char	*name;
	char	*exec_path;
	char	**args;
	int		impld;
	char	*heredoc;
	int		fd_infile;
	int		fd_outfile;
	int		err;
}	t_scmd;

typedef struct s_quote
{
	int				i;
	int				on;
	int				dq;
	struct s_quote	*next;
}	t_quote;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exp;
	struct s_env	*next;
}	t_env;

int		ft_strcmp(char *s1, char *s2);
void	env(t_env *env);
void	pwd(char *path);
void	cd(char **args, t_options	*opts, int child);
void	echo(char **args);
int		is_impld(char *name);
void	exec_impld(t_scmd *scmd, t_options *opts, int child);
t_quote	*check_quotes_pipes(t_options *opts);
void	expand_vars(char ***scmd, t_env *env, int status);
char	*expand(char **scmd, int j, t_env *env, int status);
void	expand_2(char **tmp_scmd, char **first, char **var, char **last);
void	init(t_options *opts, char **env);
char	*ft_getenv(t_env *env, char *key);
void	init_scmds(t_scmd *scmds, int n_scmds);
void	unset(t_options *opts, char **args, int child);
void	ft_exit(t_options *opts, char **args, int *status);
int		parse_input(t_options *opts);
void	exec(t_options *opts);
char	*trim_quotes(char *red);
void	unclosed_quotes(t_quote *quotes, t_options *opts);
int		condition(t_options *opts, int i, t_quote *quotes);
int		quoted(t_quote *quotes, int i);

void	export(t_options *opts, char **args, t_env **env, int exit);
void	add_var(char *key, char *value, t_env **env, int exp);
void	modify_var(char *key, char *value, t_env **env);
int		already_exist(char *key, t_env **env);
void	export_print(t_env *env);

//Redirection
int		redirect_type(char *red, t_scmd *scmd, int type, t_env *env);
int		redirect(t_scmd *cmd, t_env *env);
int		in(char *red, t_scmd *scmd);
int		out(char *red, t_scmd *scmd);
int		heredoc(char *red, t_scmd *scmd, t_env *env);
int		append(char *red, t_scmd *scmd);
void	new_alloc(char ***cmd);
void	expand_heredoc(int q, t_scmd *scmd, t_env *env);
int		remaining(int in, char *remains, char **tmp);
int		trim_redir_token(char **str);
int		search_token(char *str);

//Error checking
int		cmp(char c, int token);
int		check_errors(t_options *opts);
int		check_out_errors(t_options *opts);
int		check_in_errors(t_options *opts);
int		check_pipe_errors(t_options *opts);
int		error_msg(void);
int		check_scmds(char **scmds);
int		is_empty(char *input);
int		ft_error(char *prob, char *var, char *err);
int		check_double_token(int tk_counter, int token, char *input, int i);

void	dfree(char **str);
void	free_scmds(t_scmd **scmds, int n_scmds);
void	clean_exit(t_options *opts, int code);
void	free_quotes(t_quote *quotes);
void	free_char_array(char *p);

void	get_key_value(char *args, t_env **env);
void	no_equal(int equal, char *key, char *args, t_env **env);

#endif