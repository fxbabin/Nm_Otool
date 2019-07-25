/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_fat.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 18:04:42 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/25 19:07:34 by fbabin           ###   ########.fr       */
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
	fat = (struct fat_arch*)((size_t)env->ptr + sizeof(*header));
	tmp = (void*)(env->ptr);
	while (i < swap_uint32(header->nfat_arch))
	{
		if (swap_uint32(fat->cputype) == CPU_TYPE_X86_64)
		{
			i = -1;
			break ;
		}
		fat = (struct fat_arch*)((size_t)fat + sizeof(struct fat_arch));
		i++;
	}
	if ((int)i == -1)
	{
		env->ptr = (void*)((size_t)tmp + swap_uint32(fat->offset) );
		nm(env);
	}
	else
	{
		i = 0;
		fat = (struct fat_arch*)((size_t)env->ptr + sizeof(*header));
		while (i < swap_uint32(header->nfat_arch))
		{
			if (swap_uint32(fat->cputype) == CPU_TYPE_I386)
			{
				if (swap_uint32(header->nfat_arch) > 1)
					ft_printf("\n%s (for architecture i386):\n", env->filename);
				else
					ft_printf("%s:\n", env->filename);
				env->ptr = (void*)((size_t)tmp + swap_uint32(fat->offset) );
				nm(env);
			}
			else if (swap_uint32(fat->cputype) == CPU_TYPE_POWERPC)
			{
				if (swap_uint32(header->nfat_arch) > 1)
					ft_printf("\n%s (for architecture ppc):\n", env->filename);
				else
					ft_printf("%s:\n", env->filename);
				env->ptr = (void*)((size_t)tmp + swap_uint32(fat->offset) );
				nm(env);
			}
			fat = (struct fat_arch*)((size_t)fat + sizeof(struct fat_arch));
			i++;
		}
	}
	
	return (0);
}
