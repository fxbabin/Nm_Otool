/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_section_table32.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:26:55 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/23 18:13:28 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static char		get_sectname_letter(char *sectname)
{
	if (ft_strcmp(sectname, SECT_TEXT) == 0)
		return ('T');
	else if (ft_strcmp(sectname, SECT_DATA) == 0)
		return ('D');
	else if (ft_strcmp(sectname, SECT_BSS) == 0)
		return ('B');
	return ('S');
}

static void		alloc_sect_table32(t_env *env, struct mach_header *header)
{
	struct segment_command		*tmp;
	uint32_t					nb_sects;
	uint32_t					i;

	i = -1;
	nb_sects = 0;
	while (++i < env->ncmds)
	{
		if (env->lc->cmd == LC_SEGMENT)
		{
			tmp = ((struct segment_command*)env->lc);
			nb_sects += tmp->nsects;
		}
		env->lc = (struct load_command*)((size_t)env->lc + env->lc->cmdsize);
		++i;
	}
	env->lc = (struct load_command*)((size_t)env->ptr + sizeof(*(header)));
	if (!(env->c_sects = (char*)malloc((nb_sects + 2))))
		return ;
}

void			get_section_tablefat(t_env *env, struct mach_header *header)
{
	struct segment_command		*seg;
	struct section				*sect;
	uint32_t					idx;
	uint32_t					i;
	uint32_t					y;

	i = 0;
	idx = 0;
	alloc_sect_table32(env, header);
	while (i < env->ncmds)
	{
		if (env->lc->cmd == LC_SEGMENT
			&& ((struct segment_command*)env->lc)->nsects > 0)
		{
			seg = ((struct segment_command*)env->lc);
			sect = (struct section*)((size_t)env->lc + sizeof(*(seg)));
			y = 0;
			while (y < seg->nsects)
				env->c_sects[idx++] = get_sectname_letter(sect[y++].sectname);
		}
		env->lc = (struct load_command*)((size_t)env->lc + env->lc->cmdsize);
		i++;
	}
	env->lc = (struct load_command*)((size_t)env->ptr + sizeof(*(header)));
}
