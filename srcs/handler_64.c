/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:05:01 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/29 15:04:25 by fbabin           ###   ########.fr       */
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
		if ((arr[i]->n_type & N_TYPE) == N_UNDF)
		{
			if ((arr[i]->n_type & N_EXT) && arr[i]->n_value)
				ft_printf("%016llx %c %s\n", arr[i]->n_value, 'C', env->stringtable + arr[i]->n_un.n_strx);
			else
				ft_printf("%16c %c %s\n", ' ', (arr[i]->n_type & N_EXT) ? 'U' : 'u',
						env->stringtable + arr[i]->n_un.n_strx);
		}
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
			ft_printf("%016llx %c %s\n", arr[i]->n_value, c,
					env->stringtable + arr[i]->n_un.n_strx);
		}
	}
}

int		che(t_env *env, struct nlist_64 **arr, int begin, int last)
{
	int		i;

	i = begin - 1;
	while (++i <= last)
	{
		if (!(move_ptr(env, env->stringtable,  arr[i]->n_un.n_strx)))
			return (-1);
	}
	return (0);
}

int			check_ss(t_env *env, char *str, size_t offset)
{
	char	*tmp;
	int		i;

	i = -1;
	if (!(tmp = (char*)move_ptr(env, str, offset)))
		return (-1);
	//ft_printf("%p || %p\n", (void*)((size_t)env->start + env->file_size), str);
	while (++i > -1)
	{
		if (!(move_ptr(env, tmp, i)))
			return (-1);
		if (!tmp || !tmp[i])
			break ;
	}
	return (0);
}

int			ft_strc(t_env *env, char *string)
{
	int		ret;

	ret = 0;
	while (*string)
	{
		ret++;
		if (!(move_ptr(env, string, 1)))
			return (-1);
		string++;
	}
	return (0);
}

int			check_str(t_env *env, struct nlist_64 **arr, int end)
{
	int			i;
	char		*tmp;

	i = 0;
	while (i < end)
	{
		//ft_printf("%d\n", i);
		//ft_printf("%p || %p\n",(void*)((size_t)env->start + env->file_size),  (void*)(&(arr[i]->n_un.n_strx)));
		tmp = (char*)(env->stringtable + arr[i]->n_un.n_strx);
		//	return (-1);
		if (ft_strc(env, tmp) == -1)
		{
			return (-1);
		}
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
		return (-1);
	//ft_printf("%d\n", *((int*)env->stringtable));
	i = -1;
	//int		las;

	//las = 0;
	while (++i < env->sym->nsyms)
	{
		arr[i] = &array[i];
		if (!(move_ptr(env, env->stringtable, array[i].n_un.n_strx)))
			return (-1);
		//las = ft_strlenp((char*)(env->stringtable + array[i].n_un.n_strx));
	}
	//if (check_ss(env,env->stringtable, array[i].n_un.n_strx) == -1)
	//	return (-1);
	//ft_printf("%d || %d\n", i, env->sym->nsyms - 1);
	//arr[i] = NULL;
	if ((check_str(env, arr, env->sym->nsyms - 1)) == -1)
		return (-1);
	//if (*((char*)((size_t)env->ptr + env->file_size - 1)) == '\n')
	//	return (-1);
	//ft_printf("%p || %p\n", (void*)((size_t)env->ptr + env->file_size) , env->stringtable + array[i].n_un.n_strx);
	//if (!(move_ptr(env, env->ptr, ((size_t)env->stringtable - (size_t)env->ptr + array[i - 1].n_un.n_strx))))
	//	return (-1);

	ft_quicksort((void**)arr, 0, env->sym->nsyms - 1, env->stringtable);
	print_64(env, arr);
	free(arr);
	return (0);
}

int					handle_64(t_env *env)
{
	struct mach_header_64	*header;
	uint32_t				i;

	i = 0;
	header = (struct mach_header_64*)(env->ptr);
	env->ncmds = header->ncmds;
	if (!(env->lc = (struct load_command*)move_ptr(env, env->ptr, sizeof(*(header)))))
		return (-1);
	if ((get_section_table_64(env, header)) == -1)
		return (err_msg(-1, env->filename, "handle_64 failed"));
	while (i < env->ncmds)
	{
		if (env->lc->cmd == LC_SYMTAB)
		{
			env->sym = (struct symtab_command*)env->lc;
			if ((display_64(env)) == -1)
				return (-1);
			break ;
		}
		if (!(env->lc = (struct load_command*)move_ptr(env, env->lc, env->lc->cmdsize)))
			return (-1);
		++i;
	}
	free(env->c_sects);
	return (0);
}
