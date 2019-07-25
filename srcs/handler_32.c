/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_32.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:21:55 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/25 20:55:51 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"


static void			print_32(t_env *env, struct nlist **arr)
{
	uint32_t			i;
	char				c;

	i = -1;
	c = 'z';
	while (++i < env->sym->nsyms)
	{
		if ((arr[i]->n_type & N_STAB) != 0)
			continue ;
		if ((arr[i]->n_type & N_TYPE) == N_UNDF)
			ft_printf("%8c %c %s\n", ' ', (arr[i]->n_type & N_EXT) ? 'U' : 'u',
				env->stringtable + arr[i]->n_un.n_strx);
		else
		{
			if ((arr[i]->n_type & N_TYPE) == N_ABS)
				c = (arr[i]->n_type & N_EXT) ? 'A' : 'a';
			else if ((arr[i]->n_type & N_TYPE) == N_INDR)
				c = (arr[i]->n_type & N_EXT) ? 'I' : 'i';
			else
			{
				c = env->c_sects[arr[i]->n_sect - 1];
				c += (arr[i]->n_type & N_EXT) ? 0 : 32;
			}
			ft_printf("%08llx %c %s\n", arr[i]->n_value, c,
				env->stringtable + arr[i]->n_un.n_strx);
		}
	}
}

static int				display_32(t_env *env)
{
	struct nlist		*array;
	struct nlist		**arr;
	uint32_t			i;

	array = (struct nlist*)((size_t)env->ptr + env->sym->symoff);
	if (!(arr = (struct nlist**)malloc(
		(env->sym->nsyms + 1) * sizeof(struct nlist_64*))))
		return (err_msg(-1, env->filename, "display_32 failed"));
	env->stringtable = (char*)((size_t)env->ptr + env->sym->stroff);
	i = -1;
	while (++i < env->sym->nsyms)
		arr[i] = &array[i];
	arr[i] = NULL;
	ft_quicksort((void**)arr, 0, env->sym->nsyms - 1, env->stringtable);
	print_32(env, arr);
	free(arr);
	return (0);
}

int					handle_32(t_env *env)
{
	struct mach_header	*header;
	uint32_t			i;

	i = 0;
	header = (struct mach_header*)(env->ptr);
	env->ncmds = header->ncmds;
	env->lc = (struct load_command*)((size_t)env->ptr + sizeof(*(header)));
	if ((get_section_table_32(env, header)) == -1)
		return (err_msg(-1, env->filename, "handle_32 failed"));
	while (i < env->ncmds)
	{
		if (env->lc->cmd == LC_SYMTAB)
		{
			env->sym = (struct symtab_command*)env->lc;
			display_32(env);
			break ;
		}
		env->lc = (struct load_command*)((size_t)env->lc + env->lc->cmdsize);
		++i;
	}
	free(env->c_sects);
	return (0);
}
