/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_ar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 22:10:23 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/29 22:59:44 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"



int					handle_ar(t_env *env)
{
	void				*curr;
	int					curr_len;
	void		*start;

	if (!(curr = (void*)move_ptr(env, env->ptr, 8)))
		return (-1);
	curr_len = ft_atoi((char*)((size_t)curr + 3));
	if (!(curr = (void*)move_ptr(env, curr, 60 + curr_len)))
		return (-1);
	curr_len = *((int*)curr);
	if (!(curr = (void*)move_ptr(env, curr, 4 + curr_len)))
		return (-1);
	curr_len = *((int*)curr);
	if (!(curr = (void*)move_ptr(env, curr, 4 + curr_len)))
		return (-1);

	struct ar_hdr	*tmp;
	tmp = (struct ar_hdr*)((size_t)curr);
	start = (void*)((size_t)env->ptr);
	while ((size_t)tmp < ((size_t)start + env->arch_size))
	{
		if (!((char*)move_ptr(env, tmp, sizeof(*tmp))))
			return (-1);
		ft_printf("\n%s(%s):\n", env->filename, (char*)((size_t)tmp + sizeof(*tmp)));
		if (!(env->ptr = (void*)move_ptr(env, tmp, sizeof(*tmp) + ft_atoi((char*)((size_t)tmp + 3)))))
			return (-1);
		nm (env);
		env->ptr = start;
		tmp = (struct ar_hdr*)((size_t)tmp + 60 + ft_atoi(tmp->ar_size));
		//	return (-1);
	}
	return (0);
}
