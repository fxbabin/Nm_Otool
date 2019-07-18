/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 17:55:21 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/18 19:00:47 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <stdio.h>

static void	    print_output(uint32_t nsyms,
                    uint32_t symoff, uint32_t stroff, void *ptr)
{
	uint32_t					i;
	char				*stringtable;
	struct nlist_64		*array;

	array = (struct nlist_64*)((size_t)ptr + symoff);
	stringtable = (char*)((size_t)ptr + stroff);


	for (i=0; i < nsyms; i++)
	{
		printf("%s\n", stringtable + array[i].n_un.n_strx);
	}
}

void	handle_64(void *ptr)
{
	uint32_t					ncmds;
	uint32_t					i;
	struct mach_header_64		*header;
	struct load_command			*lc;
	struct symtab_command		*sym;

	header = (struct mach_header_64*)ptr;
	ncmds = header->ncmds;
	lc = (struct load_command*)((size_t)ptr + sizeof(*header));
	i = 0;
    while (i < ncmds)
    {
        if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			//printf("nb symbols %d\n", sym->nsyms);
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break ;
		}
		lc = (struct load_command*)((size_t)lc + lc->cmdsize);
        ++i;
    }
}
