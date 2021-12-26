#include "../includes/minishell.h"

int		cmp(char c, int token)
{
	if (c == (char)token)
		return (1);
	return (0);
}

int		check_errors(t_options *opts)
{
	check_pipe_errors(opts);
	check_in_errors(opts);
	check_out_errors(opts);
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
				printf("TOOOOO MANY IN > REDIRECTIONS !!");
			else if(out_counter == 2)
			{
				opts->input[i] = APPEND;
				opts->input[i + 1] = UNQSPACE;
			}
			while (cmp(opts->input[j],UNQSPACE))
				j++;
			if (cmp(opts->input[j],OUT) && j != i + 1)
				printf("DUP");
			else if ((cmp(opts->input[j],PIPE) || cmp(opts->input[j],IN)))
				printf("Consecutive tokens");
		}
		i++;
	}
	return (0);
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
				printf("TOOOOO MANY IN < REDIRECTIONS !!");
			else if (in_counter == 2)
			{
				opts->input[i] = HEREDOC;
				opts->input[i + 1] = UNQSPACE;
			}
			while (cmp(opts->input[j],UNQSPACE))
				j++;
			if (cmp(opts->input[j],IN) && j != i + 1)
				printf("DUP");
			else if ((cmp(opts->input[j],PIPE) || cmp(opts->input[j],OUT)))
				printf("Consecutive tokens");
		}
		i++;
	}
	return (0);
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
				printf("Duplicate PIPE");
			else if ((cmp(opts->input[j],OUT)))
				printf("Consecutive tokens");
		}
		i++;
	}
	if (cmp(opts->input[0],PIPE) || cmp(opts->input[len - 1],PIPE)
		|| cmp(opts->input[len - 1],IN)
		|| cmp(opts->input[len - 1],OUT))
		{
			printf("ERRORRR");
			return (0);
		}
	return (1);
}

// void    error_msd()