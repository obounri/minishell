#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

typedef struct s_options
{
    char    *user;
    char    *input;
    int     success;
}   t_options;


#endif