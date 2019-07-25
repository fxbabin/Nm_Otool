# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fbabin <fbabin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/17 21:49:09 by fbabin            #+#    #+#              #
#    Updated: 2019/07/25 22:16:06 by fbabin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	ft_nm

CC				=	gcc
CFLAGS			=	-Wall -Werror -Wextra #-g3 -fsanitize=address#-Weverything

_SRCS			=	ft_nm.c utils.c quicksort.c \
					handler_64.c get_section_table_64.c \
					handler_32.c get_section_table_32.c \
					handler_fat.c get_section_table_fat.c \
					handler_ppc.c get_section_table_ppc.c \
					handler_ar.c

SRCS_DIR		=	srcs
SRCS			=	$(addprefix $(SRCS_DIR)/,$(_SRCS))

OBJS_DIR		=	objs
OBJS			=	$(addprefix $(OBJS_DIR)/,$(_SRCS:%.c=%.o))

PRINTF_DIR		=	ft_printf
_PRINTF			=	libftprintf.a
PRINTF			=	$(PRINTF_DIR)/$(_PRINTF) 

INCS_DIR		=	includes
INCS			=	-I $(INCS_DIR) -I $(PRINTF_DIR)/$(INCS_DIR)
HEADER			=	$(INCS_DIR)/ft_nm.h $(PRINTF_DIR)/$(INCS_DIR)/ft_printf.h 


all: lib $(HEADER) $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS)
	@$(CC) $(CFLAGS) $(INCS) -o $(NAME) -L$(PRINTF_DIR) -lftprintf $(OBJS)
	@echo "$(NAME) : Done"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(HEADER)
	@$(CC) $(CFLAGS) $(INCS) -c -o $@ $<

lib:
	@make -C $(PRINTF_DIR)

clean:
	@make fclean -C $(PRINTF_DIR)
	@/bin/rm -rf $(OBJS_DIR)
	@echo "$(NAME) clean : Done"

fclean: clean
	@/bin/rm -f $(NAME)
	@echo "$(NAME) fclean : Done"

re:
	@make fclean
	@make

.PHONY: all clean fclean re
