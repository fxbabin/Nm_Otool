/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_section_table_ppc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 19:33:49 by fbabin            #+#    #+#             */
/*   Updated: 2019/08/01 16:31:57 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int		alloc_sect_table_ppc(t_env *env, struct mach_header *header)
{
	struct segment_command	*tmp;
	uint32_t				nb_sects;
	uint32_t				i;

	i = -1;
	nb_sects = 0;
	while (++i < env->ncmds)
	{
		if (swap_uint32(env->lc->cmd) == LC_SEGMENT)
		{
			tmp = ((struct segment_command*)env->lc);
			nb_sects += swap_uint32(tmp->nsects);
		}
		if (!(env->lc = (struct load_command*)move_ptr(env, env->lc,
			swap_uint32(env->lc->cmdsize))))
			return (-1);
		++i;
	}
	if (!(env->lc = (struct load_command*)move_ptr(env,
		env->ptr, sizeof(*(header)))))
		return (-1);
	if (!(env->c_sects = (char*)malloc((nb_sects + 2))))
		return (err_msg(-1, env->filename, "alloc_sect_table_ppc failed"));
	return (0);
}

static int		get_info(t_env *env, struct segment_command *seg,
				struct section *sect)
{
	uint32_t	y;

	y = 0;
	while (y < swap_uint32(seg->nsects))
	{
		if (ft_strcmp(sect[y].sectname, SECT_TEXT) == 0)
		{
			env->text_size = swap_uint32(sect[y].size);
			env->text_addr = swap_uint32(sect[y].addr);
			env->text_raddr = ((size_t)env->ptr + swap_uint32(sect[y].offset));
		}
		y++;
	}
	return (0);
}

static int		process_sections(t_env *env, uint32_t i)
{
	struct segment_command		*seg;
	struct section				*sect;
	uint32_t					idx;

	i = 0;
	idx = 0;
	while (i < env->ncmds)
	{
		if (swap_uint32(env->lc->cmd) == LC_SEGMENT
			&& swap_uint32(((struct segment_command*)env->lc)->nsects) > 0)
		{
			seg = ((struct segment_command*)env->lc);
			if (!(sect = (struct section*)move_ptr(env,
				env->lc, sizeof(*(seg)))))
				return (ret_free(-1, env->c_sects));
			get_info(env, seg, sect);
		}
		if (!(env->lc = (struct load_command*)move_ptr(env,
			env->lc, swap_uint32(env->lc->cmdsize))))
			return (ret_free(-1, env->c_sects));
		i++;
	}
	return (0);
}

int				get_section_table_ppc(t_env *env, struct mach_header *header)
{
	uint32_t	i;

	i = 0;
	if (alloc_sect_table_ppc(env, header) == -1)
		return (err_msg(-1, env->filename, "get_table_ppc failed"));
	if ((process_sections(env, i)) == -1)
		return (-1);
	if (!(env->lc = (struct load_command*)move_ptr(env,
		env->ptr, sizeof(*(header)))))
		return (ret_free(-1, env->c_sects));
	return (0);
}
