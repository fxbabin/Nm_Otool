/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:05:01 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/30 00:01:58 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void			print_defined(t_env *env, struct nlist_64 **arr,
						uint32_t i, char c)
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
	ft_printf("%016llx %c %s\n", arr[i]->n_value, c,
		env->stringtable + arr[i]->n_un.n_strx);
}

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
		if ((arr[i]->n_type & N_TYPE) == N_UNDF)
		{
			if ((arr[i]->n_type & N_EXT) && arr[i]->n_value)
				ft_printf("%016llx %c %s\n", arr[i]->n_value, 'C',
					env->stringtable + arr[i]->n_un.n_strx);
			else
				ft_printf("%16c %c %s\n", ' ',
					(arr[i]->n_type & N_EXT) ? 'U' : 'u',
					env->stringtable + arr[i]->n_un.n_strx);
		}
		else
			print_defined(env, arr, i, c);
	}
}

static int			check_str(t_env *env, struct nlist_64 **arr, int end)
{
	int			i;
	char		*tmp;

	i = 0;
	while (i < end)
	{
		tmp = (char*)(env->stringtable + arr[i]->n_un.n_strx);
		if (ft_strc(env, tmp) == -1)
			return (-1);
		i++;
	}
	return (0);
}

static int			display_64(t_env *env)
{
	struct nlist_64		*array;
	struct nlist_64		**arr;
	uint32_t			i;

	if (!(array = (struct nlist_64*)move_ptr(env, env->ptr, env->sym->symoff)))
		return (-1);
	if (!(arr = (struct nlist_64**)malloc(
					(env->sym->nsyms + 1) * sizeof(struct nlist_64*))))
		return (err_msg(-1, env->filename, "display_64 malloc failed"));
	if (!(env->stringtable = (char*)move_ptr(env, env->ptr, env->sym->stroff)))
		return (ret_free(-1, arr));
	i = -1;
	while (++i < env->sym->nsyms)
	{
		arr[i] = &array[i];
		if (!(move_ptr(env, env->stringtable, array[i].n_un.n_strx)))
			return (ret_free(-1, arr));
	}
	if ((check_str(env, arr, env->sym->nsyms - 1)) == -1)
		return (ret_free(-1, arr));
	ft_quicksort((void**)arr, 0, env->sym->nsyms - 1, env->stringtable);
	print_64(env, arr);
	return (ret_free(0, arr));
}

int					handle_64(t_env *env)
{
	struct mach_header_64	*header;
	int						i;

	i = -1;
	header = (struct mach_header_64*)(env->ptr);
	env->ncmds = header->ncmds;
	if (!(env->lc = (struct load_command*)move_ptr(env,
		env->ptr, sizeof(*(header)))))
		return (-1);
	if ((get_section_table_64(env, header)) == -1)
		return (err_msg(-1, env->filename, "handle_64 failed"));
	while (++i < (int)env->ncmds)
	{
		if (env->lc->cmd == LC_SYMTAB)
		{
			env->sym = (struct symtab_command*)env->lc;
			if ((display_64(env)) == -1)
				return (ret_free(-1, env->c_sects));
			break ;
		}
		if (!(env->lc = (struct load_command*)move_ptr(env,
			env->lc, env->lc->cmdsize)))
			return (ret_free(-1, env->c_sects));
	}
	return (ret_free(0, env->c_sects));
}
