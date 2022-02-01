#include "../includes/minishell.h"

int	check_scmds(char **scmds)
{
	int	i;

	i = 0;
	if (!scmds)
		return (0);
	while (scmds[i])
	{
		if (is_empty(scmds[i]) == 0)
			return (error_msg());
		i++;
	}
	return (1);
}

int	error_msg(void)
{
	ft_error("minishell", NULL, ": Parsing error");
	return (0);
}

int	cmp(char c, int token)
{
	if (c == (char)token)
		return (1);
	return (0);
}

int	is_empty(char *input)
{
	int	i;

	i = 0;
	while (input && input[i] == UNQSPACE)
		i++;
	if (!input[i])
		return (0);
	return (1);
}

int	check_errors(t_options *opts)
{
	if (is_empty(opts->input) == 0)
		return (0);
	if (check_pipe_errors(opts) == 0)
		return (0);
	if (check_in_errors(opts) == 0)
		return (0);
	if (check_out_errors(opts) == 0)
		return (0);
	return (1);
}

