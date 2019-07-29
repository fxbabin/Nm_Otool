/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_ppc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 19:23:01 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/28 23:58:56 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int		lexical_cmp64(void *a1, void *a2,
		char *stringtable)
{
	struct nlist_64		*tmp1;
	struct nlist_64		*tmp2;

	tmp1 = (struct nlist_64*)a1;
	tmp2 = (struct nlist_64*)a2;
	if (ft_strcmp(stringtable + swap_uint32(tmp1->n_un.n_strx),
				stringtable + swap_uint32(tmp2->n_un.n_strx)) == 0)
	{
		return (swap_uint32(tmp1->n_value) - swap_uint32(tmp2->n_value));
	}
	return (ft_strcmp(stringtable + swap_uint32(tmp1->n_un.n_strx),
				stringtable + swap_uint32(tmp2->n_un.n_strx)));
}

static void			ft_qs_swap2(void **arr, int left, int right)
{
	void	*tmp;

	tmp = arr[left];
	arr[left] = arr[right];
	arr[right] = tmp;
}

static int		ft_partition2(void **array, int left, int right,
		char *stringtable)
{
	void	*pivot;

	pivot = array[(left + right) / 2];
	while (left <= right)
	{
		while (lexical_cmp64(array[left], pivot, stringtable) < 0)
			left++;
		while (lexical_cmp64(array[right], pivot, stringtable) > 0)
			right--;
		if (left <= right)
		{
			ft_qs_swap2(array, left, right);
			left++;
			right--;
		}
	}
	return (left);
}

void			ft_quicksort_cigam(void **array, int left, int right,
		char *stringtable)
{
	int		index;

	if (left >= right || left < 0 || right < 0 || !array)
		return ;
	index = ft_partition2(array, left, right, stringtable);
	ft_quicksort_cigam(array, left, index - 1, stringtable);
	ft_quicksort_cigam(array, index, right, stringtable);
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
	}
}

static int			display_ppc(t_env *env)
{
	struct nlist	*array;
	struct nlist	**arr;
	uint32_t		i;

	array = (struct nlist*)((size_t)env->ptr + swap_uint32(env->sym->symoff));
	if (!(arr = (struct nlist**)malloc(
					(swap_uint32(env->sym->nsyms) + 1) * sizeof(struct nlist*))))
		return (err_msg(-1, env->filename, "display_ppc failed"));
	env->stringtable = (char*)((size_t)env->ptr + swap_uint32(env->sym->stroff));
	i = -1;
	while (++i < swap_uint32(env->sym->nsyms))
		arr[i] = &array[i];
	arr[i] = NULL;
	ft_quicksort_cigam((void**)arr, 0, swap_uint32(env->sym->nsyms) - 1, env->stringtable);
	print_ppc(env, arr);
	free(arr);
	return (0);
}

int					handle_ppc(t_env *env)
{
	struct mach_header	*header;
	uint32_t			i;

	i = 0;
	header = (struct mach_header*)(env->ptr);
	env->ncmds = swap_uint32(header->ncmds);
	env->lc = (struct load_command*)((size_t)env->ptr + sizeof(*(header)));
	if (get_section_table_ppc(env, header) == -1)
		return (err_msg(-1, env->filename, "handle_ppc failed"));
	while (i < env->ncmds)
	{
		if (swap_uint32(env->lc->cmd) == LC_SYMTAB)
		{
			env->sym = (struct symtab_command*)env->lc;
			if (display_ppc(env) == -1)
				return (err_msg(-1, env->filename, "handle_ppc failed"));
			break ;
		}
		env->lc = (struct load_command*)((size_t)env->lc + swap_uint32(env->lc->cmdsize));
		++i;
	}
	return (0);
}
