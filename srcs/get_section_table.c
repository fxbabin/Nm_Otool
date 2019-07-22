/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_section_table.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/22 17:30:54 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/22 18:40:57 by fbabin           ###   ########.fr       */
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

static void		alloc_sect_table(t_env *env)
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
		env->lc = (struct load_command*)((size_t)env->lc + env->lc->cmdsize);
		++i;
	}
	env->lc = (struct load_command*)((size_t)env->ptr + sizeof(*(env->header)));
	if (!(env->sects = (struct section_64 **)malloc(
		(nb_sects + 2) * sizeof(struct section_64*))))
		return ;
	if (!(env->c_sects = (char*)malloc((nb_sects + 2))))
		return ;
}

void			get_section_table(t_env *env)
{
	struct segment_command_64	*seg;
	struct section_64			*sect;
	uint32_t					idx;
	uint32_t					i;
	uint32_t					y;

	i = 0;
	idx = 0;
	alloc_sect_table(env);
	while (i < env->ncmds)
	{
		if (env->lc->cmd == LC_SEGMENT_64
			&& ((struct segment_command_64*)env->lc)->nsects > 0)
		{
			seg = ((struct segment_command_64*)env->lc);
			sect = (struct section_64*)((size_t)env->lc + sizeof(*(seg)));
			y = 0;
			while (y < seg->nsects)
			{
				//ft_printf("%s %c\n", sect[y].sectname, get_sectname_letter(sect[y].sectname));
				env->c_sects[idx] = get_sectname_letter(sect[y].sectname);
				env->sects[idx++] = &(sect[y++]);
			}
		}
		env->lc = (struct load_command*)((size_t)env->lc + env->lc->cmdsize);
		i++;
	}
	env->lc = (struct load_command*)((size_t)env->ptr + sizeof(*(env->header)));
}
