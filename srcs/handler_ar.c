/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_ar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 22:10:23 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/27 23:40:18 by fbabin           ###   ########.fr       */
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
	(void)env;
	//ft_printf("here\n");
	struct ar_hdr		*header;

	header = (struct ar_hdr*)((size_t)env->ptr + 8);
	//ft_printf("%s\n", (char*)((size_t)header));
	int		namelen;

	namelen = ft_atoi((char*)((size_t)header + 3));
	//ft_printf("%d\n", namelen);
	//ft_printf("%p\n", (void*)((size_t)header + sizeof(*header) + namelen - (size_t)env->ptr) );
	//ft_printf("%s\n", (char*)((size_t)header + sizeof(*header)) );
	
	int		nb_symbol;

	nb_symbol = *((int*)((size_t)header + sizeof(*header) + namelen)) / 8;
	//ft_printf("+> %d\n", nb_symbol);
	int		i;

	i = -1;
	char		*stringtable;

	stringtable = (char*)((size_t)env->ptr + 0x70C + 4);
	
	struct ranlib	*dd;

	dd = (struct ranlib*)((size_t)header + sizeof(*header) + namelen + 4);
	int nb_objs;

	uint32_t prev;

	prev = 0;
	nb_objs = 0;
	size_t first;

	first = env->file_size;
	while (++i < nb_symbol)
	{
		if (dd[i].ran_off < first)
			first = dd[i].ran_off;
		if (dd[i].ran_off != prev)
		{
			prev = dd[i].ran_off;
			nb_objs += 1;
		}
		//ft_printf("%p\n", dd[i].ran_off);
		
		//ft_printf("%s\n", stringtable + dd[i].ran_un.ran_strx);
		//ft_printf("%p || %p\n", (void*)((size_t)dd + (i * sizeof(struct ranlib))- (size_t)env->ptr), dd[i].ran_off);
		//dd = (struct ranlib*)((size_t)dd + 8);
	}
	//ft_printf("+> %d\n", nb_objs);
	struct ar_hdr	*tmp;
	size_t		curr;
	
	curr = 0;
	tmp = (struct ar_hdr*)((size_t)env->ptr + first);
	void		*start;

	start = env->ptr;
	//int		u = 0;
	while ((size_t)tmp < ((size_t)env->ptr + env->file_size))
	{
		ft_printf("\n%s(%s):\n", env->filename, (char*)((size_t)tmp + sizeof(*tmp)));
		//ft_printf("%p\n", (void*)((size_t)tmp + sizeof(*tmp) + ft_atoi((char*)((size_t)tmp + 3)) - (size_t)env->ptr)); 
		
		env->ptr = (void*)((size_t)tmp + sizeof(*tmp) + ft_atoi((char*)((size_t)tmp + 3)));
		nm (env);
		env->ptr = start;
		//break ;
		tmp = (struct ar_hdr*)((size_t)tmp + 60 + ft_atoi(tmp->ar_size));
		//ft_printf("--> %p\n", (void*)((size_t)tmp - (size_t)env->ptr));
	}
	env->ptr = start;
	/*struct ar_hdr	*tmp;
	int		y;

	y = -1;
	tmp = (struct ar_hdr*)((size_t)env->ptr + first);
	while (++y < nb_objs)
	{
		//ft_printf("%d\n", y);
		//ft_printf("%d\n", ft_atoi(tmp->ar_size));
		//ft_printf("%d\n", ft_atoi((char*)((size_t)tmp->ar_name + 3)));
		ft_printf("%s\n", (char*)((size_t)tmp + sizeof(*tmp)));
		//break ;
		//ft_printf("%s : %d\n", (char*)(tmp + 1), ft_atoi(tmp->ar_size));
		tmp = (struct ar_hdr*)((size_t)tmp + 60 + ft_atoi(tmp->ar_size));
	}
	ft_printf("%d\n", env->file_size);*/
	/*ft_printf("%p\n", (void*)(sizeof(*(header)) + 8));
	  ft_printf("%p\n", (void*)(sizeof(*(header)) + 28));
	  ft_printf("%s\n", (char*)(header->ar_mode));
	  struct ranlib		*dd;

	  dd = (struct ranlib*)((size_t)env->ptr + sizeof(*(header)) + 28);
	  ft_printf("%d\n", dd[0].ran_off);
	  ft_printf("%d\n", dd[0].ran_un.ran_strx);
	  char		*stringtable;

	  stringtable = (char*)((size_t)env->ptr + 0x70C);
	  ft_printf("%s\n", stringtable + 4);
	  */
	//env->lc = (struct load_command*)

	/*int		i;

	  i = 0;
	  ft_printf("%s\n", stringtable[i]);
	  */
	//header->ar_name[16] = 0;
	//ft_putstr_size((char*)header, 60);
	//ft_printf("%p\n", (void*)((size_t)env->ptr + sizeof(*(header))));
	//ft_printf("%p\n", (void*)(sizeof(*(header)) + 8 + 20));
	//struct ranlib		*dd;

	//dd = (struct ranlib*)((size_t)env->ptr + 88);
	//ft_printf("%s\n", dd->ran_un.ran_strx);
	//*/
	//ft_printf("%p\n", (void*)(sizeof(*(header)) + 8));

	/*struct mach_header_64	*header;
	  uint32_t				i;

	  i = 0;
	  header = (struct mach_header_64*)(env->ptr);
	  env->ncmds = header->ncmds;
	  env->lc = (struct load_command*)((size_t)env->ptr + sizeof(*(header)));
	  if ((get_section_table_64(env, header)) == -1)
	  return (err_msg(-1, env->filename, "handle_64 failed"));
	  while (i < env->ncmds)
	  {
	  if (env->lc->cmd == LC_SYMTAB)
	  {
	  env->sym = (struct symtab_command*)env->lc;
	  display_64(env);
	  break ;
	  }
	  env->lc = (struct load_command*)((size_t)env->lc + env->lc->cmdsize);
	  ++i;
	  }
	  free(env->c_sects);*/
	return (0);
}
