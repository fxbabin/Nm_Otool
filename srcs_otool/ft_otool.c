/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbabin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/30 00:00:30 by fbabin            #+#    #+#             */
/*   Updated: 2019/08/01 03:32:54 by fbabin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

static int			check_64(t_env *env, struct fat_header *header,
		struct fat_arch *fat)
{
	uint32_t			i;

	i = -1;
	while ((uint32_t)++i < swap_uint32(header->nfat_arch))
	{
		if (swap_uint32(fat->cputype) == CPU_TYPE_X86_64)
		{
			i = -2;
			env->arch_size = swap_uint32(fat->size);
			break ;
		}
		if (!(fat = (struct fat_arch*)move_ptr(env, fat,
			sizeof(struct fat_arch))))
			return (-1);
	}
	if ((int)i != -2)
		env->multi = 1;
	if (!(env->ptr = (void*)move_ptr(env, env->ptr,
			swap_uint32(fat->offset))))
			return (-1);
	//ft_printf("%p\n", ((size_t)env->ptr - (size_t)env->start));
	return (0);
}

void		print_filename(t_env *env)
{
	struct fat_header	*header;
	struct fat_arch		*fat;
	int					magic_number;

	magic_number = *(int*)(env->ptr);
	if (magic_number == (int)FAT_MAGIC || magic_number == (int)FAT_CIGAM)
	{
		env->multi = -1;
		header = (struct fat_header*)(env->ptr);
		if (!(fat = (struct fat_arch*)move_ptr(env, env->ptr, sizeof(struct fat_header))))
			return ;
		check_64(env, header, fat);
	}
	if (ft_strncmp((char*)env->ptr, ARMAG, SARMAG) == 0)
		env->multi = 2;
	if (env->multi == 2)
		ft_printf("Archive : %s\n", env->filename);
	else if (env->multi != 1)
		ft_printf("%s:\n", env->filename);
}

int			nm(t_env *env)
{
	int		magic_number;
	int		ret;

	ret = 0;
	magic_number = *(int*)(env->ptr);
	if (magic_number == (int)MH_MAGIC_64)
	{
		ft_printf("Contents of (__TEXT,__text) section\n");
		ret = handle_64(env);
	}
	else if (magic_number == (int)MH_MAGIC)
		ret = handle_32(env);
	else if (magic_number == (int)FAT_MAGIC || magic_number == (int)FAT_CIGAM)
		ret = handle_fat(env);
	else if (magic_number == (int)MH_CIGAM)
	{
		ft_printf("Contents of (__TEXT,__text) section\n");
		ret = handle_ppc(env);
	}
	else if (ft_strncmp((char*)env->ptr, ARMAG, SARMAG) == 0)
	{
		//env->ffat = 1;
		if (env->arch_size == 0)
			env->arch_size = env->file_size;
		ret = handle_ar(env);
	}
	return (ret);
}

int			process_file(t_env *env, char *filename)
{
	struct stat		buf;
	int				fd;

	env->filename = filename;
	if ((fd = open(filename, O_RDONLY)) < 0)
		return (err_msg(EXIT_FAILURE, filename, "open failed"));
	if (fstat(fd, &buf) < 0)
		return (err_msg(EXIT_FAILURE, filename, "fstat failed"));
	env->file_size = ((size_t)(buf).st_size);
	if ((env->ptr = mmap(0, ((size_t)(buf).st_size), PROT_READ,
					MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (err_msg(EXIT_FAILURE, filename, "mmap failed"));
	env->start = env->ptr;
	env->arch_size = 0;
	env->c_sects = NULL;
	if ((close(fd)) < 0)
		return (err_msg(EXIT_FAILURE, filename, "close failed"));
	print_filename(env);
	//ft_printf("%d\n", env->multi);
	if (nm(env) == -1)
		return (err_msg(EXIT_FAILURE, filename, "nm failed"));
	if (munmap(env->start, (size_t)((buf).st_size)) < 0)
		return (err_msg(EXIT_FAILURE, filename, "munmap failed"));
	return (0);
}

int			process_files(t_env *env, int argc, char **argv)
{
	int		i;

	i = 0;
	if (argc < 2)
		return (err_msg(-1, NULL, "file needed"));
	else if (argc == 2)
	{
		if (process_file(env, argv[1]) == -1)
			return (err_msg(-1, argv[1], "process file failed"));
	}
	else
	{
		while (++i < argc)
		{
			if (process_file(env, argv[i]) == -1)
				return (err_msg(-1, argv[i], "process file failed"));
		}
	}
	return (0);
}


int			main(int argc, char **argv)
{
	t_env		*env;

	(void)argc;
	(void)argv;
	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (err_msg(EXIT_FAILURE, NULL, "malloc failed on env"));
	env->buff = malloc(PBUFF_SIZE + 1);
	env->pos  = 0;
	env->multi = 0;
	env->text_size = 0;
	env->mod = 1;
	ft_memsetp(env->buff, 0, PBUFF_SIZE + 1);

	//print_address(env, 15);
	/*pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  pflush(env, "esfnkjsefnjnsfjnekjfnesj\t", 7);
	  */
	/*ft_printf("%d\n", env->b.pos);*/
	//print_address(env, 256);
	//print_address(env, 257);
	/*char		*pp;

	  pp = malloc(110);
	  ft_memsetp(pp, 0, 110);
	  pp[0] = -5;
	  print_oline(env, pp, 0);
	  write(1, &(env->b), env->b.pos);
	  */
	//print_buffer(env, "tototot", 7);
	//char str[21];

	//ft_printf("%d\n", ft_strlenp(str));
	//ft_printf("%s\n", str);*/
	if ((process_files(env, argc, argv)) == -1)
	{
		free(env);
		return (err_msg(EXIT_FAILURE, NULL, "process files failed"));
	}
	free(env->buff);
	free(env);
	return (EXIT_SUCCESS);
}
