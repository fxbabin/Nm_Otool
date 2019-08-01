# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/17 21:49:09 by fbabin            #+#    #+#              #
#    Updated: 2019/08/01 04:28:50 by fbabin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	ft_nm
NAME2			=	ft_otool

CC				=	gcc
CFLAGS			=	-Wall -Werror -Wextra #-g3 -fsanitize=address#-Weverything

_NM				=	ft_nm.c utils.c utils2.c ft_atoi.c quicksort.c \
					handler_64.c get_section_table_64.c \
					handler_32.c get_section_table_32.c \
					handler_fat.c quicksort_ppc.c \
					handler_ppc.c get_section_table_ppc.c \
					handler_ar.c \

_OTOOL			=	ft_otool.c utils.c utils2.c ft_atoi.c quicksort.c \
					handler_64.c get_section_table_64.c \
					handler_32.c get_section_table_32.c \
					handler_fat.c quicksort_ppc.c \
					handler_ppc.c get_section_table_ppc.c \
					handler_ar.c print_filename.c \

NM_DIR			=	srcs_nm
OTOOL_DIR		=	srcs_otool

NM_SRCS			=	$(addprefix $(NM_DIR)/,$(_NM))
OTOOL_SRCS		=	$(addprefix $(OTOOL_DIR)/,$(_OTOOL))

NM_ODIR			=	objs_nm
OTOOL_ODIR		=	objs_otool
NM_OBJS			=	$(addprefix $(NM_ODIR)/,$(_NM:%.c=%.o))
OTOOL_OBJS		=	$(addprefix $(OTOOL_ODIR)/,$(_OTOOL:%.c=%.o))

PRINTF_DIR		=	ft_printf
_PRINTF			=	libftprintf.a
PRINTF			=	$(PRINTF_DIR)/$(_PRINTF) 

INCS_DIR		=	includes
INCS			=	-I $(INCS_DIR) -I $(PRINTF_DIR)/$(INCS_DIR)
HEADER			=	$(INCS_DIR)/ft_otool.h $(INCS_DIR)/ft_nm.h \
					$(PRINTF_DIR)/$(INCS_DIR)/ft_printf.h

all: lib $(HEADER) $(NAME) $(NAME2)

$(NAME): $(NM_ODIR) $(NM_OBJS) $(NM_SRCS)
	@$(CC) $(CFLAGS) $(INCS) -o $(NAME) -L$(PRINTF_DIR) -lftprintf $(NM_OBJS)
	@echo "$(NAME) : Done"

$(NAME2): $(OTOOL_ODIR) $(OTOOL_OBJS) $(OTOOL_SRCS)
	@$(CC) $(CFLAGS) $(INCS) -o $(NAME2) -L$(PRINTF_DIR) -lftprintf $(OTOOL_OBJS)
	@echo "$(NAME2) : Done"

$(NM_ODIR):
	@mkdir -p $(NM_ODIR)

$(OTOOL_ODIR):
	@mkdir -p $(OTOOL_ODIR)

$(OTOOL_ODIR)/%.o: $(OTOOL_DIR)/%.c $(HEADER)
	@$(CC) $(CFLAGS) $(INCS) -c -o $@ $<

$(NM_ODIR)/%.o: $(NM_DIR)/%.c $(HEADER)
	@$(CC) $(CFLAGS) $(INCS) -c -o $@ $<

lib:
	@make -C $(PRINTF_DIR)

clean:
	@make fclean -C $(PRINTF_DIR)
	@/bin/rm -rf $(NM_ODIR)
	@/bin/rm -rf $(OTOOL_ODIR)
	@echo "$(NAME) clean : Done"

fclean: clean
	@/bin/rm -f $(NAME)
	@/bin/rm -f $(NAME2)
	@echo "$(NAME) fclean : Done"

re:
	@make fclean
	@make

.PHONY: all clean fclean re
