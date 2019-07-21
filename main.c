/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 23:58:57 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/20 16:12:18 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*tmp;

	tmp = (char*)dst;
	while (n--)
		*(tmp++) = *((char*)src++);
	return (dst);
}

static void		ft_merge(void **array, void **tmp, t_mparams par,
		int (cmp)(void*, void*))
{
	int		i;
	int		j;
	int		k;
	int		mid;

	mid = (par.right + par.left) / 2;
	i = par.left;
	j = mid + 1;
	k = 0;
	while (i <= mid && j <= par.right)
		if (cmp(array[i], array[j]) <= 0)
			tmp[k++] = array[i++];
		else
			tmp[k++] = array[j++];
	while (i <= mid)
		tmp[k++] = array[i++];
	while (j <= par.right)
		tmp[k++] = array[j++];
	while (--k >= 0)
		array[par.left + k] = tmp[k];
}

static void		ft_sort(void **array, void **tmp, t_mparams par,
		int (cmp)(void*, void*))
{
	int		mid;
	int		tmp_left;
	int		tmp_right;

	if (par.left >= par.right || par.left < 0 || par.right < 0)
		return ;
	mid = (par.right + par.left) / 2;
	tmp_left = par.left;
	tmp_right = par.right;
	par.right = mid;
	ft_sort(array, tmp, par, cmp);
	par.left = mid + 1;
	par.right = tmp_right;
	ft_sort(array, tmp, par, cmp);
	par.left = tmp_left;
	ft_merge(array, tmp, par, cmp);
}

void			ft_mergesort(void **array, int left, int right,
		int (cmp)(void*, void*))
{
	void		**tmp;
	t_mparams	par;

	if (left < 0 || right < 0 || !array || !*array)
		return ;
	if (!(tmp = (void**)malloc((right + 1) * sizeof(void*))))
		return ;
	ft_memcpy(tmp, array, (right + 1) * sizeof(void*));
	par.left = left;
	par.right = right;
	ft_sort(array, tmp, par, cmp);
	free(tmp);
}

int     main(void)
{
	int		**arr;


	arr = (int**)malloc(5*sizeof(int*));
	arr[0] = (int*)malloc(sizeof(int));
	arr[1] = (int*)malloc(sizeof(int));
	arr[2] = (int*)malloc(sizeof(int));
	arr[3] = (int*)malloc(sizeof(int));
	arr[4] = (int*)malloc(sizeof(int));
	arr[0][0] = 2;
	arr[1][0] = 5;
	arr[2][0] = 3;
	arr[3][0] = 4;
	arr[4][0] = 0;

	int		i = -1;

	while (++i < 5)
		printf("%d ", arr[i][0]);
	printf("\n");
	ft_mergesort((void**)arr, 0, 4);
	i = -1;
	while (++i < 5)
		printf("%d ", arr[i][0]);
	printf("\n");

	return (0);
}
