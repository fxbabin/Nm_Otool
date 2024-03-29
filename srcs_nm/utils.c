/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 17:04:30 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/29 23:00:48 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

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
