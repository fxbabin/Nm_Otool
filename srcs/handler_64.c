/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:05:01 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/23 15:13:14 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void			print_64(t_env *env, struct nlist_64 **arr)
{
	uint32_t			i;
	char				c;

	i = -1;
	c = 'z';
	while (++i < env->sym->nsyms)
	{
		if ((arr[i]->n_type & N_STAB) != 0)
			continue ;
		if (arr[i]->n_sect == 0)
			ft_printf("%16c %c %s\n", ' ', (arr[i]->n_type & N_EXT) ? 'U' : 'u',
				env->stringtable + arr[i]->n_un.n_strx);
		else
		{
			if (arr[i]->n_type == 3)
				c = (arr[i]->n_type & N_EXT) ? 'A' : 'a';
			else
			{
				c = env->c_sects[arr[i]->n_sect - 1];
				c += (arr[i]->n_type & N_EXT) ? 0 : 32;
			}
			ft_printf("%016llx %c %s\n", arr[i]->n_value, c,
				env->stringtable + arr[i]->n_un.n_strx);
		}
	}
}

static void			display_64(t_env *env)
{
	struct nlist_64		*array;
	struct nlist_64		**arr;
	uint32_t			i;

	array = (struct nlist_64*)((size_t)env->ptr + env->sym->symoff);
	if (!(arr = (struct nlist_64**)malloc(
		(env->sym->nsyms + 1) * sizeof(struct nlist_64*))))
		return ;
	env->stringtable = (char*)((size_t)env->ptr + env->sym->stroff);
	i = -1;
	while (++i < env->sym->nsyms)
		arr[i] = &array[i];
	arr[i] = NULL;
	ft_quicksort((void**)arr, 0, env->sym->nsyms - 1, env->stringtable);
	print_64(env, arr);
	free(arr);
}

void				handle_64(t_env *env)
{
	struct mach_header_64		*header;
	uint32_t					i;

	i = 0;
	header = (struct mach_header_64*)(env->ptr);
	env->ncmds = header->ncmds;
	env->lc = (struct load_command*)((size_t)env->ptr + sizeof(*(header)));
	get_section_table64(env, header);
	while (i < env->ncmds)
	{
		if (env->lc->cmd == LC_SYMTAB)
		{
			env->sym = (struct symtab_command*)env->lc;
			display_64(env);
			break ;
		}
		env->lc = (struct load_command*)((size_t)env->lc + env->lc->cmdsize);
		++i;
	}
}
