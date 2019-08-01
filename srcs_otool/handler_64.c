/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:05:01 by fbabin            #+#    #+#             */
/*   Updated: 2019/08/01 16:40:39 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int			pprint(t_env *env)
{
	size_t		offset;

	offset = 0;
	while (env->text_size >= 16)
	{
		print_address(env, env->text_addr + offset, 16);
		pflush(env, "\t", 1);
		if ((print_oline(env, (char*)((size_t)env->text_raddr + offset),
			16, env->mod)) == -1)
			return (-1);
		pflush(env, "\n", 1);
		offset += 16;
		env->text_size -= 16;
	}
	if (env->text_size > 0)
	{
		print_address(env, env->text_addr + offset, 16);
		pflush(env, "\t", 1);
		if ((print_oline(env, (char*)((size_t)env->text_raddr + offset),
			env->text_size, env->mod)) == -1)
			return (-1);
		pflush(env, "\n", 1);
	}
	return (0);
}

static int			display_64(t_env *env)
{
	if ((pprint(env)) == -1)
		return (-1);
	write(1, env->buff, env->pos);
	env->pos = 0;
	return (0);
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
