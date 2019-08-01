/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_filename.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 04:27:56 by fbabin            #+#    #+#             */
/*   Updated: 2019/08/01 04:30:22 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int			check_64(t_env *env, struct fat_header *header,
		struct fat_arch *fat)
{
	uint32_t			i;

	i = -1;
	while ((uint32_t)++i < swap_uint32(header->nfat_arch))
	{
		if (swap_uint32(fat->cputype) == CPU_TYPE_X86_64)
		{
			i = -2;
			env->arch_size = swap_uint32(fat->size);
			break ;
		}
		if (!(fat = (struct fat_arch*)move_ptr(env, fat,
			sizeof(struct fat_arch))))
			return (-1);
	}
	if ((int)i != -2)
		env->multi = 1;
	if (!(env->ptr = (void*)move_ptr(env, env->ptr,
		swap_uint32(fat->offset))))
		return (-1);
	return (0);
}

int					print_filename(t_env *env)
{
	struct fat_header	*header;
	struct fat_arch		*fat;
	int					magic_number;

	magic_number = *(int*)(env->ptr);
	if (magic_number == (int)FAT_MAGIC || magic_number == (int)FAT_CIGAM)
	{
		env->multi = -1;
		header = (struct fat_header*)(env->ptr);
		if (!(fat = (struct fat_arch*)move_ptr(env,
			env->ptr, sizeof(struct fat_header))))
			return (-1);
		if ((check_64(env, header, fat)) == -1)
			return (-1);
	}
	if (!(move_ptr(env, env->ptr, SARMAG)))
		return (-1);
	if (ft_strncmp((char*)env->ptr, ARMAG, SARMAG) == 0)
		env->multi = 2;
	if (env->multi == 2)
		ft_printf("Archive : %s\n", env->filename);
	else if (env->multi != 1)
		ft_printf("%s:\n", env->filename);
	return (0);
}
