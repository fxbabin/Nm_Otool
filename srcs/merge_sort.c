/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/19 00:16:57 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/22 18:28:41 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int				lexical_cmp(struct nlist_64 *a1, struct nlist_64 *a2)
{
	if (ft_strcmp(stringtable + a1->n_un.n_strx,
		stringtable + a2->n_un.n_strx) == 0)
	{
		return (a1->n_value - a2->n_value);
	}
	return (ft_strcmp(stringtable + a1->n_un.n_strx,
		stringtable + a2->n_un.n_strx));
}

static void		ft_merge(struct nlist_64 **array, struct nlist_64 **tmp,
		int left, int right)
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
		if (lexical_cmp(array[i], array[j]) <= 0)
			tmp[k++] = array[i++];
		else
			tmp[k++] = array[j++];
	while (i <= mid)
		tmp[k++] = array[i++];
	while (j <= right)
		tmp[k++] = array[j++];
	while (--k >= 0)
		array[left + k] = tmp[k];
}


static void		ft_sort(struct nlist_64 **array, struct nlist_64 **tmp, int left, int right)
{
	int		mid;

	if (left >= right || left < 0 || right < 0)
		return ;
	mid = (right + left) / 2;
	ft_sort(array, tmp, left, mid);
	ft_sort(array, tmp, mid + 1, right);
	ft_merge(array, tmp, left, right);
}

void			ft_nl64_mergesort(struct nlist_64 **array, int left, int right)
{
	struct nlist_64		**tmp;

	if (left < 0 || right < 0 || !array)
		return ;
	//ft_printf("%d || %d\n", le:w
	//ft, right);
	if (!(tmp = (struct nlist_64**)malloc((right + 1) * sizeof(struct nlist_64*))))
		return ;
	ft_memcpy(tmp, array, (right + 1) * sizeof(struct nlist_64*));
	/*int		i;

	i = -1;
	while (++i < right)
		ft_printf("%s\n", stringtable + tmp[i]->n_un.n_strx);
	tmp[i] = NULL;
	*/
	//ft_printf("%s\n", stringtable + tmp[i]->n_un.n_strx);
	//ft_printf("toto\n");
	//ft_printf("toto\n");
	ft_sort(array, tmp, left, right);
	/*int i = -1;
	while (++i < right)
		ft_printf("%s\n", stringtable + array[i]->n_un.n_strx);
	*/
	free(tmp);
}
