/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 17:04:30 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/31 16:36:07 by fbabin           ###   ########.fr       */
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

void		*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		*((char *)dst + i) = *((const char *)src + i);
		i++;
	}
	return (dst);
}

uint32_t	swap_uint32(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
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
/*
void	*ft_memcpyy(void *dst, const void *src, size_t n)
{
	char	*tmp;

	tmp = (char*)dst;
	while (n--)
		*(tmp++) = *((char*)src++);
	return (dst);
}

char	*ft_strncpy_off(char *dest, int offset, const char *src, size_t n)
{
	char			*tmp;

	tmp = dest + offset;
	while (n-- && *src)
		*(dest++) = *(src++);
	while (n-- + 1)
		*(dest++) = '\0';
	return (tmp);
}
*/

void		pflush(t_env *env, const char *str, int n)
{
	int		i;

	i = 0;
	if ((env->pos + n) >= PBUFF_SIZE)
	{
		write(1, env->buff, env->pos);
		//write(1, str, ft_strlenp(str));
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

void		print_oline(t_env *env, char *ptr, int size)
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
		//if ((i % mod) == 1 && i != 0)
		//(void)mod;
		pflush(env, " ", 1);
		i++;
	}
}

/*
   void		print_buffer(t_env *env, char *str, int size)
   {
   int		space_left;
   int		pos;

   pos = 0;
   space_left = 0;
   while ((env->bpos + size) >= PBUFF_SIZE)
   {
   space_left = (PBUFF_SIZE - env->bpos);
   ft_memcpy(env->buff + env->bpos, str + tmp, space_left);
   write(1, &(env->buff), PBUFF_SIZE);
   size -= space_left;
   tmp += space_left;
   }
   ft_memcpy(env->buff + env->bpos, str + tmp, (env->bpos + size));
   env->bpos = env->bpos + size;
   }

   void		flush_buffer(t_env *env)
   {
   write(1, env->buff, env->bpos);
   env->bpos = 0;
   }*/
