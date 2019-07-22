/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_32.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/22 19:02:04 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/22 19:09:32 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
static void		print_o(t_env *env)
{
	struct nlist_64		*array;
	struct nlist_64		**arr;
	uint32_t			i;

	array = (struct nlist_64*)((size_t)env->ptr + env->sym->symoff);
	arr = (struct nlist_64**)malloc((env->sym->nsyms + 1) * sizeof(struct nlist_64*));
	stringtable = (char*)((size_t)env->ptr + env->sym->stroff);
	i = -1;
	while (++i < env->sym->nsyms)
	{
		arr[i] = &array[i];
	}
	arr[i] = NULL;
	
	ft_nl64_mergesort(arr, 0, env->sym->nsyms - 1);
	//ft_printf("after merge\n");
	i = -1;
	char c;

	c = 'z';
	while (++i < env->sym->nsyms)
	{
		if ((arr[i]->n_type & N_STAB) != 0)
			continue ;
		//ft_printf("%d || %s\n", arr[i]->n_sect, stringtable + arr[i]->n_un.n_strx );
		if (arr[i]->n_sect == 0)
		{
			c = (arr[i]->n_type & N_EXT) ? 'U': 'u'; 
			ft_printf("%16c %c %s\n", ' ', c, stringtable + arr[i]->n_un.n_strx);
		}
		else
		{
			if (arr[i]->n_type == 3)
				c = (arr[i]->n_type & N_EXT) ? 'A': 'a';
			else
			{
				c = env->c_sects[arr[i]->n_sect - 1];
				c += (arr[i]->n_type & N_EXT) ? 0 : 32;
			}
			ft_printf("%016llx %c %s\n", arr[i]->n_value, c, stringtable + arr[i]->n_un.n_strx);
		}
		//ft_printf("%c\n", c);
	}
}*/


void	handle_32(t_env *env)
{
	uint32_t					i;
	struct mach_header		*header;

	header = (struct mach_header*)(env->ptr);
	env->ncmds = env->header->ncmds;
	env->lc = (struct load_command*)((size_t)env->ptr + sizeof(*(header)));

	i = 0;
	get_section_table32(env);
	/*while (i < env->ncmds)
	{
		if (env->lc->cmd == LC_SYMTAB)
		{
			env->sym = (struct symtab_command*)env->lc;
			print_o(env);
			break ;
		}
		env->lc = (struct load_command*)((size_t)env->lc + env->lc->cmdsize);
		++i;
	}*/
	//ft_printf("%p\n", (void*)((size_t)env->lc - (size_t)d));
	//ft_printf("%p\n", ((struct section_64*)env->lc)->addr);
	/*char	*ef = (char*)((size_t)env->ptr + 26936);
	i = -1;
	while (++i < 5)
		ft_printf("%s\n", ef[i]);
	*/
	//ft_printf("%p\n", (*)(2608));//(size_t)env->lc - (size_t)env->ptr) );
}
