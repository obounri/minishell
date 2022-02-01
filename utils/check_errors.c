/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 19:38:38 by obounri           #+#    #+#             */
/*   Updated: 2022/01/30 19:41:03 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_out_errors(t_options *opts)
{
	int	i;
	int	j;
	int	out_counter;

	i = 0;
	j = 0;
	while (opts->input[i])
	{
		if (cmp(opts->input[i], OUT))
		{
			out_counter = 1;
			j = i;
			while (cmp(opts->input[++j], OUT))
				out_counter++;
			if (out_counter > 2)
				return (error_msg());
			else if (out_counter == 2)
			{
				opts->input[i] = APPEND;
				opts->input[i + 1] = UNQSPACE;
			}
			while (cmp(opts->input[j], UNQSPACE))
				j++;
			if (cmp(opts->input[j], OUT) && j != i + 1)
				return (error_msg());
			else if ((cmp(opts->input[j], PIPE) || cmp(opts->input[j], IN)))
				return (error_msg());
		}
		i++;
	}
	return (1);
}

int	check_in_errors(t_options *opts)
{
	int	i;
	int	j;
	int	in_counter;

	i = 0;
	j = 0;
	while (opts->input[i])
	{
		if (cmp(opts->input[i], IN))
		{
			in_counter = 1;
			j = i;
			while (cmp(opts->input[++j], IN))
				in_counter++;
			if (in_counter > 2)
				return (error_msg());
			else if (in_counter == 2)
			{
				opts->input[i] = HEREDOC;
				opts->input[i + 1] = UNQSPACE;
			}
			while (cmp(opts->input[j], UNQSPACE))
				j++;
			if (cmp(opts->input[j], IN) && j != i + 1)
				return (error_msg());
			else if ((cmp(opts->input[j], PIPE) || cmp(opts->input[j], OUT)))
				return (error_msg());
		}
		i++;
	}
	return (1);
}

int	check_pipe_errors(t_options *opts)
{
	int	len;
	int	i;
	int	j;

	i = 0;
	j = 0;
	len = ft_strlen(opts->input);
	while (i < len)
	{
		if (cmp(opts->input[i], PIPE))
		{
			j = i;
			while (cmp(opts->input[++j], UNQSPACE))
				;
			if (cmp(opts->input[j], PIPE) && j != i)
				return (error_msg());
		}
		i++;
	}
	if (cmp(opts->input[0], PIPE) || cmp(opts->input[len - 1], PIPE)
		|| cmp(opts->input[len - 1], IN)
		|| cmp(opts->input[len - 1], OUT))
		return (error_msg());
	return (1);
}
