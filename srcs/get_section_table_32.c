/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_section_table32.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:26:55 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/29 21:06:00 by fbabin           ###   ########.fr       */
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

static int		alloc_sect_table_32(t_env *env, struct mach_header *header)
{
	struct segment_command	*tmp;
	uint32_t				nb_sects;
	uint32_t				i;

	i = -1;
	nb_sects = 0;
	while (++i < env->ncmds)
	{
		if (env->lc->cmd == LC_SEGMENT)
		{
			tmp = ((struct segment_command*)env->lc);
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
		return (err_msg(-1, env->filename, "alloc_table_32 malloc failed"));
	return (0);
}

static int		process_sections(t_env *env, uint32_t i, uint32_t y)
{
	struct segment_command	*seg;
	struct section			*sect;
	uint32_t				idx;

	i = 0;
	idx = 0;
	while (i < env->ncmds)
	{
		if (env->lc->cmd == LC_SEGMENT
			&& ((struct segment_command*)env->lc)->nsects > 0)
		{
			seg = ((struct segment_command*)env->lc);
			if (!(sect = (struct section*)move_ptr(env,
				env->lc, sizeof(*(seg)))))
				return (ret_free(-1, env->c_sects));
			y = 0;
			while (y < seg->nsects)
				env->c_sects[idx++] = get_sectname_letter(sect[y++].sectname);
		}
		if (!(env->lc = (struct load_command*)move_ptr(env,
			env->lc, env->lc->cmdsize)))
			return (ret_free(-1, env->c_sects));
		i++;
	}
	return (0);
}

int				get_section_table_32(t_env *env, struct mach_header *header)
{
	uint32_t	y;
	uint32_t	i;

	i = 0;
	y = 0;
	if (alloc_sect_table_32(env, header) == -1)
		return (err_msg(-1, env->filename, "get_section_table_64 failed"));
	if ((process_sections(env, i, y)) == -1)
		return (-1);
	if (!(env->lc = (struct load_command*)move_ptr(env,
		env->ptr, sizeof(*(header)))))
		return (ret_free(-1, env->c_sects));
	return (0);
}
