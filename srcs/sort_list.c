/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/21 03:10:19 by angavrel          #+#    #+#             */
/*   Updated: 2017/02/22 04:45:18 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** for -r flag
*/

t_file			*ft_reverse_lst(t_file *lst)
{
	t_file *a;
	t_file *b;
	t_file *c;

	a = NULL;
	b = lst;
	c = (lst)->next;
	while (b)
	{
		c = b->next;
		b->next = a;
		a = b;
		b = c;
	}
	return (a);
}

/*
** listing by ascii order
*/

static t_file	*lst_sort_ascii(t_file *lst)
{
	if (!lst)
		return (NULL);
	if (lst->next && ft_strcmp(lst->name, lst->next->name) > 0)
		lst = lst_swap(lst, lst->next);
	lst->next = lst_sort_ascii(lst->next);
	if (lst->next && ft_strcmp(lst->name, lst->next->name) > 0)
	{
		lst = lst_swap(lst, lst->next);
		lst->next = lst_sort_ascii(lst->next);
	}
	return (lst);
}

/*
** for -t flag
*/

static t_file	*lst_sort_time(t_file *lst)
{
	if (!lst)
		return (NULL);
	if (lst->next && (lst->time < lst->next->time))
		lst = lst_swap(lst, lst->next);
	else if (lst->next && lst->time == lst->next->time)
		if (lst->next && (lst->ntime < lst->next->ntime))
			lst = lst_swap(lst, lst->next);
	lst->next = lst_sort_time(lst->next);
	if (lst->next && (lst->time < lst->next->time))
	{
		lst = lst_swap(lst, lst->next);
		lst->next = lst_sort_time(lst->next);
	}
	else if (lst->next && lst->time == lst->next->time)
	{
		if (lst->next && (lst->ntime < lst->next->ntime))
		{
			lst = lst_swap(lst, lst->next);
			lst->next = lst_sort_time(lst->next);
		}
	}
	return (lst);
}

/*
** listing by size order
*/

static t_file	*lst_sort_size(t_file *lst)
{
	if (!lst)
		return (NULL);
	if (lst->next && (lst->size < lst->next->size))
		lst = lst_swap(lst, lst->next);
	lst->next = lst_sort_size(lst->next);
	if (lst->next && (lst->size < lst->next->size))
	{
		lst = lst_swap(lst, lst->next);
		lst->next = lst_sort_size(lst->next);
	}
	return (lst);
}

/*
** sorting list according to r and t parameters
*/

int				sort_list(t_file **begin, short flags)
{
	*begin = lst_sort_ascii(*begin);
	if (flags & LS_T)
		*begin = lst_sort_time(*begin);
	if (flags & LS_SS)
		*begin = lst_sort_size(*begin);
	if (flags & LS_R)
		*begin = ft_reverse_lst(*begin);
	return (1);
}
