/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/19 00:16:57 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/19 01:22:23 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void		ft_merge(struct nlist_64 **array, struct nlist_64 **tmp, int left, int right, char *strtable)
{
	int		i;
	int		j;
	int		k;
	int		mid;

	mid = (right + left) / 2;
	i = left;
	j = mid + 1;
	k = 0;
	while (i <= mid && j <= right)
		if (ft_strcmp(strtable + (*array)[i].n_un.n_strx, strtable + (*array)[j].n_un.n_strx) <= 0)
			((*tmp)[k++]) = (*array)[i++];
		else
			((*tmp)[k++]) = (*array)[j++];
	while (i <= mid)
		((*tmp)[k++]) = (*array)[i++];
	while (j <= right)
		((*tmp)[k++]) = (*array)[j++];
	while (--k >= 0)
		((*array)[left + k]) = (*tmp)[k];
}


static void		ft_sort(struct nlist_64 **array, struct nlist_64 **tmp, int left, int right, char *strtable)
{
	int		mid;

	if (left >= right || left < 0 || right < 0)
		return ;
	mid = (right + left) / 2;
	ft_sort(array, tmp, left, mid, strtable);
	ft_sort(array, tmp, mid + 1, right, strtable);
	ft_merge(array, tmp, left, right, strtable);
}

void			ft_nlist_mergesort(struct nlist_64 **array, int left, int right, char *strtable)
{
	struct nlist_64     *tmp;

	if (left < 0 || right < 0 || !array)
		return ;
	if (!(tmp = (struct nlist_64*)malloc((right + 1) * sizeof(struct nlist_64))))
		return ;
    ft_memcpy(tmp, array, (right + 1) * sizeof(struct nlist_64));
	ft_sort(array, &tmp, left, right, strtable);
	free(tmp);
}