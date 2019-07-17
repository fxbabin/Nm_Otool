/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 18:01:35 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/17 23:28:56 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

void	print_output(int nsyms, int symoff, int stroff, char *ptr)
{
	int					i;
	char				*stringtable;
	struct nlist_64		*array;

	array = (void*)ptr + symoff;
	stringtable = (void*)ptr + stroff;


	for (i=0; i < nsyms; i++)
	{
		printf("%s\n", stringtable + array[i].n_un.n_strx);
	}
}

void	handle_64(char *ptr)
{
	int							ncmds;
	int							i;
	struct mach_header_64		*header;
	struct load_command			*lc;
	struct symtab_command		*sym;

	header = (struct mach_header_64*)ptr;
	ncmds = header->ncmds;
	lc = (void*)ptr + sizeof(*header);
	for (i = 0; i < ncmds; ++i)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			puts("c'est le bon");
			sym = (struct symtab_command*)lc;
			printf("nb symbols %d\n", sym->nsyms);
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break ;
		}
		lc = (void*) lc + lc->cmdsize;
	}
}

void	nm(char *ptr)
{
	int		magic_number;

	magic_number = *(int*)ptr;
	if (magic_number == (int)MH_MAGIC_64)
	{
		printf("Je suis un binaire 64 bits\n");
		handle_64(ptr);
	}
}

int		main(int argc, char **argv)
{
	int		fd;
	char	*ptr;
	struct stat buf;

	if (argc != 2)
	{
		fprintf(stderr, "Please give me an arg\n");
		return (EXIT_FAILURE);
	}
	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
	if (fstat(fd, &buf) < 0)
	{
		perror("fstat");
		return (EXIT_FAILURE);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		perror("mmap");
		return (EXIT_FAILURE);
	}
	nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
	{
		perror("munmap");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
