/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 21:58:29 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/22 19:06:26 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

/*
** --------------------------------- INCLUDES ---------------------------------
*/

#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include "ft_printf.h"

/*
 ** --------------------------------- DEFINES ---------------------------------
 */

/*
** ------------------------------- STRUCTURES ---------------------------------
*/

typedef struct					s_env
{
	void						*ptr;
	size_t						file_size;
	struct mach_header_64		*header;
	struct load_command			*lc;
	struct symtab_command		*sym;
	uint32_t					ncmds;
	char						*c_sects;
	struct section_64			**sects;
}								t_env;

extern char						*stringtable;

/*
** ----------------------------------------------------------------------------
** -------------------------------- PROTOTYPES --------------------------------
** ----------------------------------------------------------------------------
*/

void							get_section_table(t_env *env);
void							handle_64(t_env *env);
void							handle_32(t_env *env);

void							ft_nl64_mergesort(struct nlist_64 **array, int left, int right);

int								ft_strcmp(const char *s1, const char *s2);
void							*ft_memcpy(void *dst, const void *src,
									size_t n);

#endif
