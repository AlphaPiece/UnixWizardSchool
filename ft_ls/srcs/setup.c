/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zwang <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/05 10:00:47 by zwang             #+#    #+#             */
/*   Updated: 2019/04/12 11:03:54 by Zexi Wang        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

extern int	g_options[OPTION_NUM];

t_obj		*create_new_obj(char *name)
{
	t_obj	*obj;

	if (!(obj = (t_obj *)malloc(sizeof(t_obj))))
		malloc_error();
	obj->name = ft_strdup(name);
	obj->super_obj = NULL;
	obj->sub_obj = NULL;
	obj->sub_dir_name = NULL;
	obj->sub_obj_num = 0;
	obj->sub_dir_num = 0;
	return (obj);
}

char		*get_path_name(t_obj *obj)
{
	char	*path_name;
	char	*ptr;

	path_name = NULL;
	while (obj)
	{
		if (path_name == NULL)
			path_name = ft_strdup(obj->name);
		else
		{
			ptr = path_name;
			path_name = ft_strcompose(3, obj->name, "/", path_name);
			if (ptr)
				free(ptr);
		}
		obj = obj->super_obj;
	}
	return (path_name);
}

static void	set_dir_lst(t_obj *obj, char *dir_lst[], int *i)
{
	int			j;
	struct stat	fs;
	char		*path_name;

	j = -1;
	while (obj->sub_obj[++j])
	{
		if (!g_options[all] && ft_strstart(obj->sub_obj[j]->name, "."))
			continue ;
		path_name = get_path_name(obj->sub_obj[j]);
		if (lstat(path_name, &fs) < 0)
			lstat_error(path_name);
		if (S_ISDIR(fs.st_mode) && !ft_strequ(obj->sub_obj[j]->name, ".") &&
			!ft_strequ(obj->sub_obj[j]->name, ".."))
			dir_lst[(*i)++] = obj->sub_obj[j]->name;
		free(path_name);
	}
}

void		set_sub_dir_name(t_obj *obj)
{
	int		i;
	char	*dir_lst[obj->sub_obj_num];

	i = 0;
	set_dir_lst(obj, dir_lst, &i);
	dir_lst[i] = NULL;
	if (!(obj->sub_dir_name = (char **)malloc(sizeof(char *) * (i + 1))))
		malloc_error();
	i = 0;
	while ((obj->sub_dir_name[i] = dir_lst[i]))
		i++;
	obj->sub_dir_num = i;
}

/*
** n[0]: current position in the dir array
** n[1]: current length of the dir array
*/

void		set_sub_obj(t_obj *obj)
{
	DIR				*dir;
	struct dirent	*dirent;
	int				n[2];
	char			*path_name;

	n[1] = OBJ_NUM;
	if (!(obj->sub_obj = (t_obj **)malloc(sizeof(t_obj *) * (OBJ_NUM + 1))))
		malloc_error();
	path_name = get_path_name(obj);
	if (!(dir = opendir(path_name)))
		opendir_error(path_name);
	free(path_name);
	n[0] = 0;
	while ((dirent = readdir(dir)))
	{
		if (n[0] + 1 == n[1])
			if (!(obj->sub_obj = (t_obj **)ft_memrealloc((void **)&obj->sub_obj,
					n[1], sizeof(t_obj *) * (n[1] + OBJ_NUM))))
				malloc_error();
		n[1] += OBJ_NUM;
		obj->sub_obj[n[0]] = create_new_obj(dirent->d_name);
		obj->sub_obj[n[0]++]->super_obj = obj;
	}
	closedir(dir);
	obj->sub_obj[n[0]] = NULL;
	obj->sub_obj_num = n[0];
}
