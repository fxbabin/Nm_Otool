/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 21:58:29 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/19 01:22:55 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

/*
** --------------------------------- INCLUDES ---------------------------------
*/

#include <mach-o/loader.h>
#include <mach-o/nlist.h>
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


/*
** ----------------------------------------------------------------------------
** -------------------------------- PROTOTYPES --------------------------------
** ----------------------------------------------------------------------------
*/

void	    handle_64(void *ptr);

void		ft_nlist_mergesort(struct nlist_64 **array, int left, int right, char *strtable);

int		    ft_strcmp(const char *s1, const char *s2);
void	    *ft_memcpy(void *dst, const void *src, size_t n);

#endif
