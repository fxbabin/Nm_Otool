/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 17:55:21 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/19 01:20:33 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
//#include <stdio.h>
#include <string.h>

//void		get_sorted_array

static void	    print_output(uint32_t nsyms,
                    uint32_t symoff, uint32_t stroff, void *ptr)
{
	uint32_t			i;
	char				*stringtable;
	struct nlist_64		*array;

	array = (struct nlist_64*)((size_t)ptr + symoff);
	stringtable = (char*)((size_t)ptr + stroff);
    (void)nsyms;
	//printf("%d || %d || %d\n", (int)nsyms, (int)symoff, (int)stroff);
 	//printf("x=> %s\n", stringtable + array[1].n_un.n_strx);
	struct nlist_64		*arr;
	
	arr = (struct nlist_64*)malloc(nsyms * sizeof(struct nlist_64));
	ft_memcpy(arr, array, nsyms * sizeof(struct nlist_64));
	ft_nlist_mergesort(&arr, 0, (int)(nsyms - 1), stringtable);
	for (i=1; i < nsyms; i++)
	{
		//arr[i] = array[i];
		//ft_printf("%s\n", stringtable + arr[i].n_type);
		if (ft_strcmp((stringtable + arr[i].n_type), "") != 0)
 			ft_printf("%016llx T %s\n", arr[i].n_value, stringtable + arr[i].n_un.n_strx);
		else
 			ft_printf("%16c U %s\n", ' ', stringtable + arr[i].n_un.n_strx);
	}
	free(arr);
	/*for (i=1; i < nsyms; i++)
	{
		ft_printf("%s\n", stringtable + array[i].n_type);
		if (strcmp((stringtable + array[i].n_type), "") != 0)
 			ft_printf("%016llx T %s\n", array[i].n_value, stringtable + array[i].n_un.n_strx);
		else
 			ft_printf("%16c U %s\n", ' ', stringtable + array[i].n_un.n_strx);
	}*/
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
