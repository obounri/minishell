/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <obounri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 17:35:32 by obounri           #+#    #+#             */
/*   Updated: 2021/11/02 17:22:59 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void    prompt(t_options *opts)
{
	if (opts->success)
		printf("\033[0;32m");
	else
		printf("\033[0;31m");
	printf("➤ ");
	printf("\033[0m");
	printf(" %s ~ ", opts->user);
	opts->input = readline("");
}

// void		catch(int sig)
// {
// 	printf("\n");
// }

int main(void)
{
	t_options	opts;
	
	opts.user = readline("Enter user name for prompt: ");
	opts.success = 1;
	while (1)
	{
		// signal(SIGINT, &catch);
		prompt(&opts);
		if (opts.success)
			opts.success = 0;
		else
			opts.success = 1;
	}
	return (0);
}