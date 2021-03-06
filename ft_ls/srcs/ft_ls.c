/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zwang <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 10:12:14 by zwang             #+#    #+#             */
/*   Updated: 2019/02/20 18:03:51 by Zexi Wang        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int			g_options[OPTION_NUM] = {0};

static void	collect_options(int *argc, char *(*argv[]))
{
	char	c;

	while (--*argc > 0 && (*++(*argv))[0] == '-')
		while ((c = *++(*argv)[0]))
			if (c == '-')
				return ;
			else if (c == 'l')
				g_options[long_format] = true;
			else if (c == 'R')
				g_options[recursion] = true;
			else if (c == 'a')
				g_options[all] = true;
			else if (c == 'r')
				g_options[reverse] = true;
			else if (c == 't')
				g_options[date] = true;
			else
			{
				ft_printf("ft_ls: illegal option -- %c\n", c);
				ft_printf("usage: ./ft_ls [-Ralrt] [file ...]\n");
				exit(1);
			}
}

int			main(int argc, char *argv[])
{
	collect_options(&argc, &argv);
	if (argc == 0)
		list_cur_dir();
	else
		list_arg(argc, argv);
	return (0);
}
