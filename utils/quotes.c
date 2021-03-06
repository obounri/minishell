/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 19:13:43 by obounri           #+#    #+#             */
/*   Updated: 2022/01/30 19:15:26 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	new_quote(t_quote **quotes, int i, int on, int dq)
{
	t_quote	*quote;
	t_quote	*tmp;

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

void	handle_quotes(t_quote **quotes, char *quote, int i, int *dq)
{
	if (quote[i] == '"' && (*dq != 0))
	{
		if (*dq == 1)
			*dq = -1;
		else if (*dq == -1)
			*dq = 1;
		new_quote(quotes, i, *dq, 1);
		quote[i] = DQ;
	}
	else if (quote[i] == '\'' && (*dq != 1))
	{
		if (*dq == 0)
			*dq = -1;
		else if (*dq == -1)
			*dq = 0;
		new_quote(quotes, i, *dq, 0);
		quote[i] = SQ;
	}
}

int	quoted(t_quote *quotes, int i)
{
	t_quote	*tmp;

	if (!quotes)
		return (0);
	tmp = quotes;
	while (tmp->next)
		tmp = tmp->next;
	if (i)
		if (tmp->on == 1 && tmp->dq == 1)
			return (0);
	if (tmp->on == 0)
		return (0);
	return (1);
}

t_quote	*check_quotes_pipes(t_options *opts)
{
	t_quote	*quotes;
	int		i;
	int		dq;

	quotes = NULL;
	dq = -1;
	i = -1;
	while (opts->input[++i])
	{
		if (opts->input[i] == '"' || opts->input[i] == '\'')
			handle_quotes(&quotes, opts->input, i, &dq);
		else if (opts->input[i] == '|' && !quoted(quotes, 0))
			opts->input[i] = PIPE;
		else if (opts->input[i] == ' ' && !quoted(quotes, 0))
			opts->input[i] = UNQSPACE;
		else if (condition(opts, i, quotes))
			opts->input[i] = EXPAND;
		else if (opts->input[i] == '<' && !quoted(quotes, 0))
			opts->input[i] = IN;
		else if (opts->input[i] == '>' && !quoted(quotes, 0))
			opts->input[i] = OUT;
	}
	unclosed_quotes(quotes, opts);
	return (quotes);
}

int	condition(t_options *opts, int i, t_quote *quotes)
{
	return (opts->input[i] == '$' && opts->input[i + 1]
		&& (ft_isalnum(opts->input[i + 1])
			|| opts->input[i + 1] == '?'
			|| opts->input[i + 1] == '_') && !quoted(quotes, 1));
}
