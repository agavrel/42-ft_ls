/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_detailed_list.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/21 03:09:19 by angavrel          #+#    #+#             */
/*   Updated: 2017/02/22 07:00:31 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** print list with -l option
*/

int		display_detailed_list(t_file *lst, int flags)
{
	int	size[7];
	int	blocks;
	int	first;

	ft_bzero(size, sizeof(size));
	blocks = 0;
	first = 1;
	get_row_size(lst, size, &blocks);
	while (lst)
	{
		if (first == 1 && !(flags & LS_D))
		{
			ft_printf("total %d\n", blocks);
			first = 0;
		}
		display_list_items(lst, size, flags);
		ft_putchar('\n');
		lst = lst->next;
	}
	return (1);
}

/*
** function to calculate row size
*/

int		get_row_size(t_file *file, int size[7], int *blocks)
{
	int	len;

	size[5] = 3;
	size[6] = 3;
	while (file)
	{
		size[0] = MAX(integer_len(file->st_blocks), size[0]);
		size[1] = MAX(integer_len(file->st_nlink), size[1]);
		size[2] = MAX(ft_strlen(getpwuid(file->st_uid)->pw_name), size[2]);
		size[3] = MAX(ft_strlen(getgrgid(file->st_gid)->gr_name), size[3]);
		if (!S_ISCHR(file->mode))
			len = integer_len(file->size);
		else
		{
			size[5] = MAX(integer_len(major(file->st_rdev)), size[5]);
			size[6] = MAX(integer_len(minor(file->st_rdev)), size[6]);
			len = size[5] + size[6] + 2;
		}
		size[4] = MAX(len, size[4]);
		*blocks += file->st_blocks;
		file = file->next;
	}
	size[5] = MAX(size[4] - size[6] - 1, size[5]);
	return (1);
}

/*
** returns number's len
*/

int		integer_len(int n)
{
	int		len;

	len = 1;
	while (n / 10)
	{
		n /= 10;
		++len;
	}
	return (len);
}
