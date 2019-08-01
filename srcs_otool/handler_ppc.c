/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_ppc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 19:23:01 by fbabin            #+#    #+#             */
/*   Updated: 2019/08/01 04:22:16 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void			pprint(t_env *env)
{
	size_t		offset;

	offset = 0;
	while (((int)env->text_size) >= 16)
	{
		print_address(env, env->text_addr + offset, 8);
		pflush(env, "\t", 1);
		print_oline(env, (char*)((size_t)env->text_raddr + offset), 16, 4);
		pflush(env, "\n", 1);
		offset += 16;
		env->text_size -= 16;
	}
	if (env->text_size > 0)
	{
		print_address(env, env->text_addr + offset, 8);
		pflush(env, "\t", 1);
		print_oline(env, (char*)((size_t)env->text_raddr + offset),
			env->text_size, 4);
		pflush(env, "\n", 1);
	}
	write(1, env->buff, env->pos);
	env->pos = 0;
}

static int			display_ppc(t_env *env)
{
	pprint(env);
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
