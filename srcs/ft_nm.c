/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 18:01:35 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/18 22:32:23 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
//#include <stdio.h>

static void		nm(void *ptr)
{
	int		magic_number;

	magic_number = *(int*)ptr;
	if (magic_number == (int)MH_MAGIC_64)
	{
		ft_printf("File is a 64 bits file\n");
		handle_64(ptr);
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

static int		nm_runner(int fd, struct stat *buf, void *ptr)
{
	if (fstat(fd, buf) < 0)
	{
		ft_dprintf(2, "Error :: fstat failed\n");
		return (EXIT_FAILURE);
	}
	if ((ptr = mmap(0, ((size_t)(*buf).st_size), PROT_READ,
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
	nm(ptr);
	if (munmap(ptr, (size_t)((*buf).st_size)) < 0)
	{
		ft_dprintf(2, "error :: close/munmap failed\n");
		return (-1);
	}
	return (0);
}

int		main(int argc, char **argv)
{
	struct stat buf;
	void		*ptr;
	int			fd;

	ptr = NULL;
	if ((fd = open_file(argc, argv)) == -1)
		return (EXIT_FAILURE);	
	if ((nm_runner(fd, &buf, ptr)) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
