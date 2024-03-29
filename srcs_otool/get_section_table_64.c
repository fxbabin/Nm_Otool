/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_section_table.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/22 17:30:54 by fbabin            #+#    #+#             */
/*   Updated: 2019/08/01 16:31:46 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int		alloc_sect_table_64(t_env *env, struct mach_header_64 *header)
{
	struct segment_command_64	*tmp;
	uint32_t					nb_sects;
	uint32_t					i;

	i = -1;
	nb_sects = 0;
	while (++i < env->ncmds)
	{
		if (env->lc->cmd == LC_SEGMENT_64)
		{
			tmp = ((struct segment_command_64*)env->lc);
			nb_sects += tmp->nsects;
		}
		if (!(env->lc = (struct load_command*)move_ptr(env, env->lc,
			env->lc->cmdsize)))
			return (-1);
		++i;
	}
	if (!(env->lc = (struct load_command*)move_ptr(env, env->ptr,
		sizeof(*(header)))))
		return (-1);
	if (!(env->c_sects = (char*)malloc((nb_sects + 2))))
		return (err_msg(-1, env->filename, "alloc_table_64 malloc failed"));
	return (0);
}

static int		get_info(t_env *env, struct segment_command_64 *seg,
				struct section_64 *sect)
{
	uint32_t	y;

	y = 0;
	while (y < seg->nsects)
	{
		if (ft_strcmp(sect[y].sectname, SECT_TEXT) == 0)
		{
			env->text_size = sect[y].size;
			env->text_addr = sect[y].addr;
			env->text_raddr = ((size_t)env->ptr + sect[y].offset);
		}
		y++;
	}
	return (0);
}

static int		process_sections(t_env *env, uint32_t i)
{
	struct segment_command_64	*seg;
	struct section_64			*sect;
	uint32_t					idx;

	i = 0;
	idx = 0;
	while (i < env->ncmds)
	{
		if (env->lc->cmd == LC_SEGMENT_64
			&& ((struct segment_command_64*)env->lc)->nsects > 0)
		{
			seg = ((struct segment_command_64*)env->lc);
			if (!(sect = (struct section_64*)move_ptr(env,
				env->lc, sizeof(*(seg)))))
				return (ret_free(-1, env->c_sects));
			get_info(env, seg, sect);
		}
		if (!(env->lc = (struct load_command*)move_ptr(env,
			env->lc, env->lc->cmdsize)))
			return (ret_free(-1, env->c_sects));
		i++;
	}
	return (0);
}

int				get_section_table_64(t_env *env, struct mach_header_64 *header)
{
	uint32_t	i;

	i = 0;
	if (alloc_sect_table_64(env, header) == -1)
		return (err_msg(-1, env->filename, "get_section_table_64 failed"));
	if ((process_sections(env, i)) == -1)
		return (-1);
	if (!(env->lc = (struct load_command*)move_ptr(env,
		env->ptr, sizeof(*(header)))))
		return (ret_free(-1, env->c_sects));
	return (0);
}
