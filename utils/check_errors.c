#include "../includes/minishell.h"

int		cmp(char c, int token)
{
	if (c == (char)token)
		return (1);
	return (0);
}

int		is_empty(char *input)
{
	int i;

	i = 0;
	while (input && input[i] == UNQSPACE)
		i++;
	if (!input[i])
		return (0);
	return (1);
}

int		check_errors(t_options *opts)
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
				return (error_msg());
			else if(out_counter == 2)
			{
				opts->input[i] = APPEND;
				opts->input[i + 1] = UNQSPACE;
			}
			while (cmp(opts->input[j],UNQSPACE))
				j++;
			if (cmp(opts->input[j],OUT) && j != i + 1)
				return (error_msg());
			else if ((cmp(opts->input[j],PIPE) || cmp(opts->input[j],IN)))
				return (error_msg());
		}
		i++;
	}
	return (1);
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
				return (error_msg());
			else if (in_counter == 2)
			{
				opts->input[i] = HEREDOC;
				opts->input[i + 1] = UNQSPACE;
			}
			while (cmp(opts->input[j],UNQSPACE))
				j++;
			if (cmp(opts->input[j],IN) && j != i + 1)
				return (error_msg());
			else if ((cmp(opts->input[j],PIPE) || cmp(opts->input[j],OUT)))
				return (error_msg());
		}
		i++;
	}
	return (1);
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
				return (error_msg());
		}
		i++;
	}
	if (cmp(opts->input[0],PIPE) || cmp(opts->input[len - 1],PIPE)
		|| cmp(opts->input[len - 1],IN)
		|| cmp(opts->input[len - 1],OUT))
				return (error_msg());
	return (1);
}

int		check_scmds(char **scmds)
{
	int i;

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

int    error_msg()
{
	ft_error("minishell", NULL, ": Parsing error");
	return (0);
}