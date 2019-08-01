/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_otool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 16:48:08 by fbabin            #+#    #+#             */
/*   Updated: 2019/08/01 16:53:12 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

void			pflush(t_env *env, const char *str, int n)
{
	int		i;

	i = 0;
	if ((env->pos + n) >= PBUFF_SIZE)
	{
		write(1, env->buff, env->pos);
		env->pos = 0;
		pflush(env, str, n);
	}
	else
	{
		ft_memcpyp((char*)((size_t)env->buff + env->pos), str, n);
		env->pos += n;
	}
}

void			print_address(t_env *env, size_t addr, int base)
{
	char	str[21];
	char	ret[17];
	int		rest;
	int		i;

	i = 0;
	rest = 0;
	ft_llutoa_base_static((char*)&str, addr, "0123456789abcdef");
	while (addr > 0 && (rest = rest + 1))
		addr /= 16;
	str[rest] = '\0';
	while ((rest + i) < base)
		ret[i++] = '0';
	ret[i] = '\0';
	pflush(env, ret, i);
	pflush(env, str, rest);
}

int				print_oline(t_env *env, char *ptr, int size, int mod)
{
	char			str[3];
	unsigned char	c;
	int				i;

	i = 0;
	while (i < size)
	{
		if (!(move_ptr(env, ptr, i)))
			return (-1);
		c = (unsigned char)ptr[i];
		ft_llutoa_base_static((char*)&str, (int)c, "0123456789abcdef");
		if (c < 16)
		{
			pflush(env, "0", 1);
			pflush(env, str, 1);
		}
		else
			pflush(env, str, 2);
		if (((i + 1) % mod) == 0)
			pflush(env, " ", 1);
		i++;
	}
	return (0);
}

static int		check_64(t_env *env, struct fat_header *header,
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

int				print_filename(t_env *env)
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
	if (ft_strncmp((char*)env->ptr, ARMAG, SARMAG) == 0)
		env->multi = 2;
	if (env->multi == 2)
		ft_printf("Archive : %s\n", env->filename);
	else if (env->multi != 1)
		ft_printf("%s:\n", env->filename);
	return (0);
}
