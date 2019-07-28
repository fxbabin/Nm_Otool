/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_ar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 22:10:23 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/28 20:25:27 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
   static void			print_64(t_env *env, struct nlist_64 **arr)
   {
   uint32_t			i;
   char				c;

   i = -1;
   c = 'z';
   while (++i < env->sym->nsyms)
   {
   if ((arr[i]->n_type & N_STAB) != 0)
   continue ;
   if ((arr[i]->n_type & N_TYPE) == N_UNDF)
   ft_printf("%16c %c %s\n", ' ', (arr[i]->n_type & N_EXT) ? 'U' : 'u',
   env->stringtable + arr[i]->n_un.n_strx);
   else
   {
   if ((arr[i]->n_type & N_TYPE) == N_ABS)
   c = (arr[i]->n_type & N_EXT) ? 'A' : 'a';
   else if ((arr[i]->n_type & N_TYPE) == N_INDR)
   c = (arr[i]->n_type & N_EXT) ? 'I' : 'i';
   else
   {
   c = env->c_sects[arr[i]->n_sect - 1];
   c += (arr[i]->n_type & N_EXT) ? 0 : 32;
   }
   ft_printf("%016llx %c %s\n", arr[i]->n_value, c,
   env->stringtable + arr[i]->n_un.n_strx);
   }
   }
   }

   static int			display_64(t_env *env)
   {
   struct nlist_64		*array;
   struct nlist_64		**arr;
   uint32_t			i;

   array = (struct nlist_64*)((size_t)env->ptr + env->sym->symoff);
   if (!(arr = (struct nlist_64**)malloc(
   (env->sym->nsyms + 1) * sizeof(struct nlist_64*))))
   return (err_msg(-1, env->filename, "display_64 malloc failed"));
   env->stringtable = (char*)((size_t)env->ptr + env->sym->stroff);
   i = -1;
   while (++i < env->sym->nsyms)
   arr[i] = &array[i];
   arr[i] = NULL;
   ft_quicksort((void**)arr, 0, env->sym->nsyms - 1, env->stringtable);
   print_64(env, arr);
   free(arr);
   return (0);
   }
   */

void	ft_putstr_size(char *str, size_t size)
{
	write(1, str, size);
}

int		ft_isspace(char c)
{
		c = (unsigned char)c;
			if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
							|| c == '\r')
						return (1);
				return (0);
}

int		ft_isdigit(int c)
{
		return (c >= '0' && c <= '9');
}

int		ft_atoi(const char *str)
{
	int					sign;
	int					a;
	unsigned long long	limit;

	limit = 0;
	a = 0;
	while (ft_isspace(*str))
		str++;
	sign = (*str == '-') ? -1 : 1;
	str += (*str == '-' || *str == '+') ? 1 : 0;
	while (*str && ft_isdigit(*str))
	{
		limit = limit * 10 + (*(str) - '0');
		if (limit > 9223372036854775807)
			return ((sign == 1) ? -1 : 0);
		a = (a * 10) + (*(str++) - '0');
	}
	return (sign * a);
}

int					handle_ar(t_env *env)
{
	void				*curr;
	int					curr_len;
	void		*start;

	curr = (void*)((size_t)env->ptr + 8);
	curr_len = ft_atoi((char*)((size_t)curr + 3));
	curr = (void*)((size_t)curr + 60 + curr_len);
	curr_len = *((int*)curr);
	curr = (void*)((size_t)curr + 4 + curr_len);
	curr_len = *((int*)curr);
	curr = (void*)((size_t)curr + 4 + curr_len);
	
	struct ar_hdr	*tmp;
	tmp = (struct ar_hdr*)((size_t)curr);
	start = (void*)((size_t)env->ptr);
	
	while ((size_t)tmp < ((size_t)start + env->arch_size))
	{
		ft_printf("\n%s(%s):\n", env->filename, (char*)((size_t)tmp + sizeof(*tmp)));
		env->ptr = (void*)((size_t)tmp + sizeof(*tmp) + ft_atoi((char*)((size_t)tmp + 3)));
		nm (env);
		env->ptr = start;
		tmp = (struct ar_hdr*)((size_t)tmp + 60 + ft_atoi(tmp->ar_size));
	}
	return (0);
}
