/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_list_items.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/21 03:09:46 by angavrel          #+#    #+#             */
/*   Updated: 2017/02/22 07:00:18 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** function to get file type
*/

static char		get_file_type(int mode)
{
	mode = (mode & S_IFMT);
	if (S_ISREG(mode))
		return ('-');
	else if (S_ISDIR(mode))
		return ('d');
	else if (S_ISLNK(mode))
		return ('l');
	else if (S_ISBLK(mode))
		return ('b');
	else if (S_ISCHR(mode))
		return ('c');
	else if (S_ISSOCK(mode))
		return ('s');
	else if (S_ISFIFO(mode))
		return ('p');
	else
		return ('-');
}

/*
** function to get number of links
*/

static char		get_file_acl(char path[PATH_MAX])
{
	acl_t	tmp;
	char	buf[101];

	if (listxattr(path, buf, sizeof(buf), XATTR_NOFOLLOW) > 0)
		return ('@');
	if ((tmp = acl_get_link_np(path, ACL_TYPE_EXTENDED)))
	{
		acl_free(tmp);
		return ('+');
	}
	return (' ');
}

/*
** get CHMOD information
*/

static void		display_chmod(char chmod[12], int mode, char path[PATH_MAX])
{
	chmod[0] = get_file_type(mode);
	chmod[1] = (S_IRUSR & mode) ? 'r' : '-';
	chmod[2] = (S_IWUSR & mode) ? 'w' : '-';
	chmod[3] = (S_IXUSR & mode) ? 'x' : '-';
	chmod[4] = (S_IRGRP & mode) ? 'r' : '-';
	chmod[5] = (S_IWGRP & mode) ? 'w' : '-';
	chmod[6] = (S_IXGRP & mode) ? 'x' : '-';
	chmod[7] = (S_IROTH & mode) ? 'r' : '-';
	chmod[8] = (S_IWOTH & mode) ? 'w' : '-';
	chmod[9] = (S_IXOTH & mode) ? 'x' : '-';
	chmod[10] = get_file_acl(path);
	chmod[11] = '\0';
	if (S_ISUID & mode)
		chmod[3] = chmod[3] == '-' ? 'S' : 's';
	if (S_ISGID & mode)
		chmod[6] = chmod[6] == '-' ? 'S' : 's';
	if (S_ISVTX & mode)
		chmod[9] = chmod[9] == '-' ? 'T' : 't';
	ft_putstr(chmod);
}

/*
** display time
** s + 4 to trim day
** %.12s in order to remove seconds and year.
*/

static void		display_time(t_file *file)
{
	time_t	today;
	char	*s;

	time(&today);
	s = ctime(&(file->time)) + 4;
	ft_printf(" %.12s ", s);
}

/*
** function used for -l option
** name is not display is file type is a link
*/

int				display_list_items(t_file *file, int size[7], int flags)
{
	char	str[12];
	char	buf[NAME_MAX + 1];

	(flags & LS_S) ? ft_printf("%*hu ", size[0], file->st_blocks) : 0;
	display_chmod(str, file->mode, file->full_path);
	ft_printf(" %*hu", size[1], file->st_nlink);
	ft_printf(" %-*s", size[2], getpwuid(file->st_uid)->pw_name);
	ft_printf("  %-*s", size[3], getgrgid(file->st_gid)->gr_name);
	if (str[0] != 'c' && str[0] != 'b')
		ft_printf("  %*lld", size[4], file->size);
	else
	{
		ft_printf(" %*d", size[5], major(file->st_rdev));
		ft_printf(", %*d", size[6], minor(file->st_rdev));
	}
	display_time(file);
	if (str[0] != 'l')
		display_name(file, flags, 0);
	else
	{
		ft_bzero(buf, NAME_MAX + 1);
		readlink(file->full_path, buf, NAME_MAX);
		ft_printf("%s -> %s", file->name, buf);
	}
	return (1);
}
