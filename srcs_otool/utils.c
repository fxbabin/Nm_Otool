/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 17:04:30 by fbabin            #+#    #+#             */
/*   Updated: 2019/08/01 04:20:39 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int			ft_strcmp(const char *s1, const char *s2)
{
	const unsigned char		*tmp1;
	const unsigned char		*tmp2;

	tmp1 = (const unsigned char *)s1;
	tmp2 = (const unsigned char *)s2;
	while (*tmp1 || *tmp2)
	{
		if (*tmp1 != *tmp2)
			return (*tmp1 - *tmp2);
		tmp1++;
		tmp2++;
	}
	return (0);
}

int			err_msg(int ret, char *filename, char *msg)
{
	if (filename)
		ft_dprintf(2, "%kError%k :: %k%s%k : %s\n",
				LRED, RESET, EOC, filename, RESET, msg);
	else
		ft_dprintf(2, "%kError%k :: %s\n",
				LRED, RESET, msg);
	return (ret);
}

void		pflush(t_env *env, const char *str, int n)
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

void		print_address(t_env *env, size_t addr, int base)
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

void		print_oline(t_env *env, char *ptr, int size, int mod)
{
	char			str[3];
	unsigned char	c;
	int				i;

	i = 0;
	while (i < size)
	{
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
}
