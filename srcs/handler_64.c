/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 17:55:21 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/22 00:00:09 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void		print_o(t_env *env)
{
	struct nlist_64		*array;
	struct nlist_64		**arr;
	//char				*stringtable;
	uint32_t			i;

	//t_str_nlist64		**arr_l64;

	array = (struct nlist_64*)((size_t)env->ptr + env->sym->symoff);
	arr = (struct nlist_64**)malloc((env->sym->nsyms + 1) * sizeof(struct nlist_64*));
	//ft_memcpy(arr, array, env->sym->nsyms * sizeof(struct nlist_64));
	stringtable = (char*)((size_t)env->ptr + env->sym->stroff);
	i = -1;
	//if (!(arr_l64 = (t_str_nlist64**)malloc(sym->nsyms * sizeof(t_str_nlist64*))))
	//return ;
	//int		i;

	i = -1;
	while (++i < env->sym->nsyms)
	{
		arr[i] = &array[i];
	}
	arr[i] = NULL;
	
	//ft_printf("before merge\n");
	ft_nl64_mergesort(arr, 0, env->sym->nsyms - 1);
	//ft_printf("after merge\n");
	i = -1;
	char c;

	c = 'z';
	while (++i < env->sym->nsyms)
	{
		if (arr[i]->n_type > 30)
			continue ;
		ft_printf("%d || %s\n", arr[i]->n_sect, stringtable + arr[i]->n_un.n_strx );
		/*if (arr[i]->n_sect == 0)
		{
			ft_printf("%16c U %s\n", ' ', stringtable + arr[i]->n_un.n_strx);
		}
		else
		{
			if (arr[i]->n_sect == 1)
			{
				c = (arr[i]->n_type == 15) ? 'T': 't';
				ft_printf("%016llx %c %s\n", arr[i]->n_value, c, stringtable + arr[i]->n_un.n_strx);
			}
			else if (arr[i]->n_sect == 12 && arr[i]->n_type == 14)
			{
				ft_printf("%016llx %c %s\n", arr[i]->n_value, 'b', stringtable + arr[i]->n_un.n_strx);
					
			}
			else if (arr[i]->n_sect == 12 || arr[i]->n_sect == 4 || arr[i]->n_sect == 5 || arr[i]->n_sect == 10 || arr[i]->n_sect == 13 )
			{
				c = (arr[i]->n_type == 15) ? 'S': 's';
				//ft_printf("%d || %d || %d || %s\n", arr[i]->n_sect, arr[i]->n_desc, arr[i]->n_type, stringtable + arr[i]->n_un.n_strx );
				ft_printf("%016llx %c %s\n", arr[i]->n_value, c, stringtable + arr[i]->n_un.n_strx);
			}
			else if (arr[i]->n_sect == 11)
			{
				c = (arr[i]->n_type == 15) ? 'D': 'd';
				ft_printf("%016llx %c %s\n", arr[i]->n_value, c, stringtable + arr[i]->n_un.n_strx);
			}
			else
				ft_printf("%d || %d || %d || %s\n", arr[i]->n_sect, arr[i]->n_desc, arr[i]->n_type, stringtable + arr[i]->n_un.n_strx );
		}
		*/
	}
}


void	handle_64(t_env *env)
{
	uint32_t					i;

	env->header = (struct mach_header_64*)(env->ptr);
	env->ncmds = env->header->ncmds;
	ft_printf("%d\n", sizeof(*(env->header)));
	env->lc = (struct load_command*)((size_t)env->ptr + sizeof(*(env->header)));
	ft_printf("%d\n", env->header->sizeofcmds);
	i = 0;

	struct section_64	*ss;

	//ss = (struct section_64*)((size_t)env->ptr + 26);
	//ft_printf("%s\n", (char*)ss->segname);
	//int d = (int)env->lc - sizeof(*(env->header));
	while (i < env->ncmds)
	{
		//ft_printf("%d\n", (int)env->lc);
		//ft_printf("%p\n", (void*)((size_t)env->lc - (size_t)d));
		if (env->lc->cmd == LC_SEGMENT_64)
		{
			ft_printf("%s\n", ((struct segment_command_64*)env->lc)->segname);
			ss = (struct section_64*)((size_t)env->lc + 72);

		}
		if (env->lc->cmd == LC_SYMTAB)
		{
			env->sym = (struct symtab_command*)env->lc;
			print_o(env);
			break ;
		}
		//if (env->lc->cmd == LC_DATA_IN_CODE)
		//	ft_printf("frsf\n");
		env->lc = (struct load_command*)((size_t)env->lc + env->lc->cmdsize);
		++i;
	}
	//ft_printf("%p\n", (void*)((size_t)env->lc - (size_t)d));
	//ft_printf("%p\n", ((struct section_64*)env->lc)->addr);
	/*char	*ef = (char*)((size_t)env->ptr + 26936);
	i = -1;
	while (++i < 5)
		ft_printf("%s\n", ef[i]);
	*/
	//ft_printf("%p\n", (*)(2608));//(size_t)env->lc - (size_t)env->ptr) );
}
