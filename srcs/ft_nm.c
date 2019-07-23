/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 18:01:35 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/23 15:13:38 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void		nm(t_env *env)
{
	int		magic_number;

	magic_number = *(int*)(env->ptr);
	if (magic_number == (int)MH_MAGIC_64)
		handle_64(env);
	else if (magic_number == (int)MH_MAGIC)
		ft_printf("32 file\n");
	else if (magic_number == (int)FAT_CIGAM)
	{
		ft_printf("fat file\n");
	}
}

static int		open_file(int argc, char **argv)
{
	int		fd;

	fd = -1;
	if (argc != 2)
	{
		ft_dprintf(2, "Error :: file needed\n");
		return (-1);
	}
	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		ft_dprintf(2, "Error :: file open failed\n");
		return (-1);
	}
	return (fd);
}

static int		nm_runner(t_env *env, int fd, struct stat *buf)
{
	if (fstat(fd, buf) < 0)
	{
		ft_dprintf(2, "Error :: fstat failed\n");
		return (EXIT_FAILURE);
	}
	env->file_size = ((size_t)(*buf).st_size);
	if ((env->ptr = mmap(0, ((size_t)(*buf).st_size), PROT_READ,
		MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_dprintf(2, "Error :: mmap failed\n");
		return (EXIT_FAILURE);
	}
	if ((close(fd)) < 0)
	{
		ft_dprintf(2, "Error :: close failed\n");
		return (EXIT_FAILURE);
	}
	nm(env);
	if (munmap(env->ptr, (size_t)((*buf).st_size)) < 0)
	{
		ft_dprintf(2, "error :: close/munmap failed\n");
		return (-1);
	}
	return (0);
}

int				main(int argc, char **argv)
{
	struct stat buf;
	int			fd;
	t_env		*env;

	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (EXIT_FAILURE);
	if ((fd = open_file(argc, argv)) == -1)
	{
		free(env);
		return (EXIT_FAILURE);
	}
	if ((nm_runner(env, fd, &buf)) == -1)
	{
		free(env);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
