/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_fat.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 18:04:42 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/28 23:28:23 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int					handle_fat(t_env *env)
{
	struct fat_header	*header;
	struct fat_arch		*fat;
	void				*tmp;
	uint32_t			i;

	i = 0;
	header = (struct fat_header*)(env->ptr);
	if (!(fat = (struct fat_arch*)move_ptr(env, env->ptr, sizeof(*header))))
		return (-1);
	tmp = (void*)(env->ptr);
	while (i < swap_uint32(header->nfat_arch))
	{
		if (swap_uint32(fat->cputype) == CPU_TYPE_X86_64)
		{
			i = -1;
			env->arch_size = swap_uint32(fat->size);
			//ft_printf("%d\n", env->arch_size);
			break ;
		}
		if (!(fat = (struct fat_arch*)move_ptr(env, fat, sizeof(struct fat_arch))))
			return (-1);
		i++;
	}
	if ((int)i == -1)
	{
		if (!(env->ptr = (void*)move_ptr(env, tmp, swap_uint32(fat->offset) )))
			return (-1);
		nm(env);
	}
	else
	{
		i = 0;
		if (!(fat = (struct fat_arch*)move_ptr(env, env->ptr, sizeof(*header))))
			return (-1);
		while (i < swap_uint32(header->nfat_arch))
		{
			if (swap_uint32(fat->cputype) == CPU_TYPE_I386)
			{
				env->arch_size = swap_uint32(fat->size);
				//ft_printf("%d\n", env->arch_size);
				if (swap_uint32(header->nfat_arch) > 1)
					ft_printf("\n%s (for architecture %s):\n", env->filename, (swap_uint32(fat->cpusubtype) == CPU_SUBTYPE_I386_ALL) ? "i386" : "");
				else
					ft_printf("%s:\n", env->filename);
				if (!(env->ptr = (void*)move_ptr(env, tmp, swap_uint32(fat->offset))))
					return (-1);
				nm(env);
			}
			else if (swap_uint32(fat->cputype) == CPU_TYPE_POWERPC)
			{
				env->arch_size = swap_uint32(fat->size);
				//ft_printf("%d\n", env->arch_size);
				if (swap_uint32(header->nfat_arch) > 1)
					ft_printf("\n%s (for architecture %s):\n", env->filename, (swap_uint32(fat->cpusubtype) == CPU_SUBTYPE_POWERPC_ALL) ? "ppc" : "" );
				else
					ft_printf("%s:\n", env->filename);
				if (!(env->ptr = (void*)((size_t)tmp + swap_uint32(fat->offset))))
					return (-1);
				nm(env);
			}
			if (!(fat = (struct fat_arch*)((size_t)fat + sizeof(struct fat_arch))))
				return (-1);
			i++;
		}
	}
	return (0);
}
