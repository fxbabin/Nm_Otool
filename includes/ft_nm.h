/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 21:58:29 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/23 15:12:56 by fbabin           ###   ########.fr       */
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

void							get_section_table64(t_env *env,
									struct mach_header_64 *header);
void							handle_64(t_env *env);
void							handle_32(t_env *env);

void							ft_quicksort(void **array, int left, int right,
									char *stringtable);
int								ft_strcmp(const char *s1, const char *s2);
void							*ft_memcpy(void *dst, const void *src,
									size_t n);

#endif
