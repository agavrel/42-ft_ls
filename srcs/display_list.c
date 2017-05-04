/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/21 03:09:33 by angavrel          #+#    #+#             */
/*   Updated: 2017/02/22 06:55:05 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** displays exactly as LIBC LS, from top to bottom and left to right instead of
** left to right and then top to bottom.
*/

static int	lst_column(t_file *lst, t_index row_col, t_index maxlen, int flags)
{
	int				tmp_column;
	int				tmp_row;
	t_file			*lst_tmp;
	int				counter;

	counter = row_col.y;
	while (lst && counter--)
	{
		tmp_column = row_col.x;
		lst_tmp = lst->next;
		while (lst && tmp_column--)
		{
			display_name(lst, flags, maxlen.y);
			if (tmp_column)
				ft_putnchar(MAX(maxlen.x - ft_strlen(lst->name), 0), ' ');
			tmp_row = row_col.y;
			while (lst && tmp_row--)
				lst = lst->next;
		}
		ft_putchar('\n');
		lst = lst_tmp;
	}
	return (1);
}

/*
** bonus -s to display blocks
*/

int			lst_blocks_len(t_file *lst, int *total)
{
	int				maxlen;

	maxlen = 0;
	while (lst)
	{
		maxlen = MAX(integer_len(lst->st_blocks), maxlen);
		*total += lst->st_blocks;
		lst = lst->next;
	}
	return (maxlen);
}

/*
** bonus on columns
** padding is done according to the longest file name (maxlen.x)
** maxlen.y is used for -s option to calculate the width of blocks display
** so we need to go through the list once in order to get maxlen (tmp).
** if (!(flags & 128) checks for -1 option, row will be number of element.
*/

static int	display_basic_list(t_file *lst, int flags)
{
	t_index			maxlen;
	struct ttysize	ts;
	t_index			i;
	t_file			*tmp_lst;
	int				total;

	total = 0;
	maxlen.x = lst_maxlen(lst) + ((flags & LS_G) ? 1 : 4);
	maxlen.y = (flags & LS_S) ? lst_blocks_len(lst, &total) : 0;
	if (flags & LS_S)
		ft_printf("total %d\n", total);
	ioctl(0, TIOCGWINSZ, &ts);
	i.x = (!(flags & 128)) ? ts.ts_cols / (maxlen.x + maxlen.y) : 1;
	i.y = 0;
	tmp_lst = lst;
	while (tmp_lst)
	{
		++i.y;
		tmp_lst = tmp_lst->next;
	}
	if (!(flags & 128))
		i.y = (i.y % i.x ? 1 : 0) + (i.y / i.x);
	lst_column(lst, i, maxlen, flags);
	return (1);
}

/*
** print list ordered as specified by user
*/

int			display_list(t_file **lst, int flags)
{
	sort_list(lst, flags);
	if (!(flags & LS_L))
		display_basic_list(*lst, flags);
	else
		display_detailed_list(*lst, flags);
	return (1);
}

/*
** sub function for norm and to handle -c color flag
*/

void		display_name(t_file *l, int flags, int blocks_len)
{
	if (blocks_len)
	{
		ft_putnchar(MAX(blocks_len - integer_len(l->st_blocks), 0), ' ');
		ft_printf("%d ", l->st_blocks);
	}
	if (flags & LS_G)
	{
		if (S_ISDIR(l->mode))
			ft_printf("%{cyan}%s%{eoc}", l->name);
		else
			ft_printf((S_IXUSR & l->mode) ? "%{red}%s%{eoc}" : "%s", l->name);
	}
	else
		ft_printf("%s", l->name);
}
