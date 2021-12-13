#include "../includes/minishell.h"

void	new_quote(t_quote **quotes, int i, int on, int dq)
{
	t_quote *quote, *tmp;

	quote = malloc(sizeof(t_quote) * 1);
	quote->i = i;
	if (on >= 0)
		quote->on = 1;
	else
		quote->on = 0;
	quote->dq = dq;
	quote->next = NULL;
	if (!*quotes)
		*quotes = quote;
	else
	{
		tmp = *quotes;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = quote;
	}
}

void    handle_quotes(t_quote  **quotes, char quote, int i, int *dq)
{
    if (quote == '"' && (*dq != 0))
    {
        if (*dq == 1)
            *dq = -1;
        else if (*dq == -1)
            *dq = 1;
        new_quote(quotes, i, *dq, 1);
    }
    else if (quote == '\''  && (*dq != 1))
    {
        if (*dq == 0)
            *dq = -1;
        else if (*dq == -1)
            *dq = 0;
        new_quote(quotes, i, *dq, 0);
    }
}

void    handle_pipe(t_quote *quotes, int i)
{
    t_quote *tmp;

    if (!quotes)
    {
        printf("split pipe at %d\n", i);
        return ;
    }
    tmp = quotes;
    while (tmp->next)
        tmp = tmp->next;
    if (tmp->on == 0)
        printf("split pipe at %d\n", i);
}

t_quote *check_quotes_pipes(t_options	*opts)
{
	int i;
	int dq;
	t_quote *quotes;
	
	quotes = NULL;
	dq = -1;
	i = 0;
	while(opts->input[i])
	{
		if (opts->input[i] == '"' || opts->input[i] == '\'')
            handle_quotes(&quotes, opts->input[i], i, &dq);
        else if (opts->input[i] == '|')
            handle_pipe(quotes, i);
        // check for unclosed quote;
		i++;
	}
	return (quotes);
}