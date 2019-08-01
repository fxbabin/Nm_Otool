/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_32.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:21:55 by fbabin            #+#    #+#             */
/*   Updated: 2019/08/01 04:46:52 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static void			pprint(t_env *env)
{
	size_t		offset;

	offset = 0;
	while (env->text_size >= 16)
	{
		print_address(env, env->text_addr + offset, 8);
		pflush(env, "\t", 1);
		if (!((void*)move_ptr(env, (void*)env->text_raddr, offset)))
			return ;
		print_oline(env, (char*)((size_t)env->text_raddr + offset),
			16, env->mod);
		pflush(env, "\n", 1);
		offset += 16;
		env->text_size -= 16;
	}
	if (env->text_size > 0)
	{
		print_address(env, env->text_addr + offset, 8);
		pflush(env, "\t", 1);
		if (!((void*)move_ptr(env, (void*)env->text_raddr, offset)))
			return ;
		print_oline(env, (char*)((size_t)env->text_raddr + offset),
			env->text_size, env->mod);
		pflush(env, "\n", 1);
	}
}

static int			display_32(t_env *env)
{
	ft_printf("Contents of (__TEXT,__text) section\n");
	pprint(env);
	write(1, env->buff, env->pos);
	env->pos = 0;
	return (0);
}

int					handle_32(t_env *env)
{
	struct mach_header	*header;
	int					i;

	i = -1;
	header = (struct mach_header*)(env->ptr);
	env->ncmds = header->ncmds;
	if (!(env->lc = (struct load_command*)move_ptr(env,
		env->ptr, sizeof(*(header)))))
		return (-1);
	if ((get_section_table_32(env, header)) == -1)
		return (err_msg(-1, env->filename, "handle_32 failed"));
	while (++i < (int)env->ncmds)
	{
		if (env->lc->cmd == LC_SYMTAB)
		{
			env->sym = (struct symtab_command*)env->lc;
			if (display_32(env) == -1)
				return (ret_free(-1, env->c_sects));
			break ;
		}
		if (!(env->lc = (struct load_command*)move_ptr(env,
			env->lc, env->lc->cmdsize)))
			return (ret_free(-1, env->c_sects));
	}
	return (ret_free(0, env->c_sects));
}
