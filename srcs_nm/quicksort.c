/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quicksort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 14:09:10 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/23 14:29:59 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int		lexical_cmp64(void *a1, void *a2,
							char *stringtable)
{
	struct nlist_64		*tmp1;
	struct nlist_64		*tmp2;

	tmp1 = (struct nlist_64*)a1;
	tmp2 = (struct nlist_64*)a2;
	if (ft_strcmp(stringtable + tmp1->n_un.n_strx,
		stringtable + tmp2->n_un.n_strx) == 0)
	{
		return (tmp1->n_value - tmp2->n_value);
	}
	return (ft_strcmp(stringtable + tmp1->n_un.n_strx,
		stringtable + tmp2->n_un.n_strx));
}

void			ft_qs_swap(void **arr, int left, int right)
{
	void	*tmp;

	tmp = arr[left];
	arr[left] = arr[right];
	arr[right] = tmp;
}

static int		ft_partition(void **array, int left, int right,
					char *stringtable)
{
	void	*pivot;

	pivot = array[(left + right) / 2];
	while (left <= right)
	{
		while (lexical_cmp64(array[left], pivot, stringtable) < 0)
			left++;
		while (lexical_cmp64(array[right], pivot, stringtable) > 0)
			right--;
		if (left <= right)
		{
			ft_qs_swap(array, left, right);
			left++;
			right--;
		}
	}
	return (left);
}

void			ft_quicksort(void **array, int left, int right,
					char *stringtable)
{
	int		index;

	if (left >= right || left < 0 || right < 0 || !array)
		return ;
	index = ft_partition(array, left, right, stringtable);
	ft_quicksort(array, left, index - 1, stringtable);
	ft_quicksort(array, index, right, stringtable);
}
