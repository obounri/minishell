/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obounri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 22:37:14 by obounri           #+#    #+#             */
/*   Updated: 2019/11/11 05:08:46 by obounri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list *s;

	if (!(s = (t_list *)malloc(sizeof(t_list))))
		return (NULL);
	(*s).content = content;
	(*s).next = NULL;
	return (s);
}
