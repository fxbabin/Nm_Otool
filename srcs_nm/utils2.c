/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 22:07:00 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/29 22:08:49 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		ft_strc(t_env *env, char *string)
{
	int		ret;

	ret = 0;
	while (*string)
	{
		ret++;
		if (!(move_ptr(env, string, 1)))
			return (-1);
		string++;
	}
	return (0);
}

int		ret_free(int ret, void *ptr)
{
	if (ptr)
		free(ptr);
	return (ret);
}

void	*move_ptr(t_env *env, void *ptr, size_t add)
{
	if (((size_t)ptr + add) >= ((size_t)env->start + env->file_size))
	{
		ft_dprintf(2, "%kError%k :: %k%s%k : corrupted file\n",
			LRED, RESET, EOC, env->filename, RESET);
		return (NULL);
	}
	return ((void*)((size_t)ptr + add));
}

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	const unsigned char		*tmp1;
	const unsigned char		*tmp2;

	tmp1 = (const unsigned char *)s1;
	tmp2 = (const unsigned char *)s2;
	while (n-- && (*tmp1 || *tmp2))
	{
		if (*tmp1 != *tmp2)
			return (*tmp1 - *tmp2);
		tmp1++;
		tmp2++;
	}
	return (0);
}
