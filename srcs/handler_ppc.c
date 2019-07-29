/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_ppc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 19:23:01 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/29 22:02:01 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void			print_defined(t_env *env, struct nlist **arr,
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
	ft_printf("%08x %c %s\n", swap_uint32(arr[i]->n_value), c,
			env->stringtable + swap_uint32(arr[i]->n_un.n_strx));
}

static void			print_ppc(t_env *env, struct nlist **arr)
{
	uint32_t			i;
	char				c;

	i = -1;
	c = 'z';
	while (++i < swap_uint32(env->sym->nsyms))
	{
		if ((arr[i]->n_type & N_STAB) != 0)
			continue ;
		if ((arr[i]->n_type & N_TYPE) == N_UNDF)
			ft_printf("%8c %c %s\n", ' ', (arr[i]->n_type & N_EXT) ? 'U' : 'u',
					env->stringtable + swap_uint32(arr[i]->n_un.n_strx));
		else
			print_defined(env, arr, i, c);
	}
}

static int			check_str(t_env *env, struct nlist **arr, int end)
{
	int			i;
	char		*tmp;

	i = 0;
	while (i < end)
	{
		tmp = (char*)(env->stringtable + swap_uint32(arr[i]->n_un.n_strx));
		if (ft_strc(env, tmp) == -1)
			return (-1);
		i++;
	}
	return (0);
}

static int			display_ppc(t_env *env)
{
	struct nlist	*array;
	struct nlist	**arr;
	uint32_t		i;

	if (!(array = (struct nlist*)move_ptr(env, env->ptr,
		swap_uint32(env->sym->symoff))))
		return (-1);
	if (!(arr = (struct nlist**)malloc(
		(swap_uint32(env->sym->nsyms) + 1) * sizeof(struct nlist*))))
		return (err_msg(-1, env->filename, "display_ppc failed"));
	if (!(env->stringtable = (char*)move_ptr(env, env->ptr,
		swap_uint32(env->sym->stroff))))
		return (-1);
	i = -1;
	while (++i < swap_uint32(env->sym->nsyms))
		arr[i] = &array[i];
	arr[i] = NULL;
	if ((check_str(env, arr, swap_uint32(env->sym->nsyms) - 1)) == -1)
		return (-1);
	ft_quicksort_cigam((void**)arr, 0, swap_uint32(env->sym->nsyms) - 1,
		env->stringtable);
	print_ppc(env, arr);
	free(arr);
	return (0);
}

int					handle_ppc(t_env *env)
{
	struct mach_header	*header;
	int					i;

	i = -1;
	header = (struct mach_header*)(env->ptr);
	env->ncmds = swap_uint32(header->ncmds);
	if (!(env->lc = (struct load_command*)move_ptr(env,
		env->ptr, sizeof(*(header)))))
		return (-1);
	if (get_section_table_ppc(env, header) == -1)
		return (err_msg(-1, env->filename, "handle_ppc failed"));
	while (++i < (int)env->ncmds)
	{
		if (swap_uint32(env->lc->cmd) == LC_SYMTAB)
		{
			env->sym = (struct symtab_command*)env->lc;
			if (display_ppc(env) == -1)
				return (ret_free(-1, env->c_sects));
			break ;
		}
		if (!(env->lc = (struct load_command*)move_ptr(env,
			env->lc, swap_uint32(env->lc->cmdsize))))
			return (ret_free(-1, env->c_sects));
	}
	return (ret_free(0, env->c_sects));
}
