/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 23:58:57 by fbabin            #+#    #+#             */
/*   Updated: 2019/07/19 17:00:26 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

typedef struct 	s_mparams
{
	int			left;
	int			right;	
}				t_mparams;

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*tmp;

	tmp = (char*)dst;
	while (n--)
		*(tmp++) = *((char*)src++);
	return (dst);
}

static void		ft_merge(void **array, void **tmp, int left, int right)
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
		if (*((int*)array[i]) <= *((int*)array[j]))
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

static void		ft_sort(void **array, void **tmp, t_mparams par)
{
	int		mid;

	if (par.left >= par.right || par.left < 0 || par.right < 0)
		return ;
	mid = (par.right + par.left) / 2;
	
	ft_sort(array, tmp, left, mid);
	ft_sort(array, tmp, mid + 1, right);
	ft_merge(array, tmp, left, right);
}

void			ft_mergesort(void **array, int left, int right)
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
	/*int	i = -1;
	while (++i < 5)
		printf("%d ", ((int*)tmp[i])[0]);
	printf("\n");
	*/
	ft_sort(array, tmp, par);
	free(tmp);
}

int     main(void)
{
    /*arr[1].n_un.n_strx = "e";
    arr[2].n_un.n_strx = "d";
    arr[3].n_un.n_strx = "a";
    arr[4].n_un.n_strx = "b";
    */
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
	
	/*printf("%d || %d\n", *arr[2], *arr[3]);   
	ft_swap((void**)&arr[2], (void**)&arr[3]);
	printf("%d || %d\n", *arr[2], *arr[3]);   
	*/
	int		i = -1;

	while (++i < 5)
		printf("%d ", arr[i][0]);
	printf("\n");
	ft_mergesort((void**)arr, 0, 4);
	i = -1;
	while (++i < 5)
		printf("%d ", arr[i][0]);
	printf("\n");


	/*char		**arr2;


	arr2 = (char**)malloc(5*sizeof(char*));
	arr2[0] = (char*)malloc(10 * sizeof(char));
	arr2[1] = (char*)malloc(10 * sizeof(char));
	arr2[2] = (char*)malloc(10 * sizeof(char));
	arr2[3] = (char*)malloc(10 * sizeof(char));
	arr2[4] = (char*)malloc(10 * sizeof(char));
	arr2[0] = "b";
	arr2[1] = "e";
	arr2[2] = "c";
	arr2[3] = "d";
	arr2[4] = "a";
	i = -1;

	while (++i < 5)
		printf("%s ", arr2[i]);
	printf("\n");
	ft_quicksort2((void**)arr2, 0, 4, ft_strcmp);
	i = -1;

	while (++i < 5)
		printf("%s ", arr2[i]);
	printf("\n");*/
	return (0);
}