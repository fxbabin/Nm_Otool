/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 21:58:29 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/30 20:35:55 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

/*
** --------------------------------- INCLUDES ---------------------------------
*/

# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach-o/ranlib.h>
# include <mach/machine.h>
# include <ar.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <unistd.h>
# include "ft_printf.h"

/*
** --------------------------------- DEFINES ---------------------------------
*/

/*
** ------------------------------- STRUCTURES ---------------------------------
*/

typedef struct					s_env
{
	char						*filename;
	void						*ptr;
	void						*start;
	size_t						file_size;
	size_t						arch_size;
	struct load_command			*lc;
	struct symtab_command		*sym;
	uint32_t					ncmds;
	char						*stringtable;
	char						*c_sects;
}								t_env;

/*
** ----------------------------------------------------------------------------
** -------------------------------- PROTOTYPES --------------------------------
** ----------------------------------------------------------------------------
*/

int								nm(t_env *env);

int								handle_64(t_env *env);
int								handle_32(t_env *env);
int								handle_fat(t_env *env);
int								handle_ppc(t_env *env);
int								handle_ar(t_env *env);

int								get_section_table_64(t_env *env,
									struct mach_header_64 *header);
int								get_section_table_32(t_env *env,
									struct mach_header *header);
int								get_section_table_ppc(t_env *env,
									struct mach_header *header);

void							ft_quicksort(void **array, int left, int right,
									char *stringtable);
void							ft_quicksort_cigam(void **array, int left,
									int right, char *stringtable);

int								ft_atoi(const char *str);
int								ft_strc(t_env *env, char *string);
void							*move_ptr(t_env *env, void *ptr, size_t add);

int								ret_free(int ret, void *ptr);
int								ft_strncmp(const char *s1, const char *s2,
									size_t n);
int								ft_strcmp(const char *s1, const char *s2);
void							*ft_memcpy(void *dst, const void *src,
									size_t n);
uint32_t						swap_uint32(uint32_t val);
int								err_msg(int ret, char *filename, char *msg);

#endif
