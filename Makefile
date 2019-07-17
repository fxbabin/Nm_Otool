# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fbabin <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/17 21:49:09 by fbabin            #+#    #+#              #
#    Updated: 2019/07/17 22:33:12 by fbabin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	ft_nm

CC				=	gcc
CFLAGS			=	-Wall -Werror -Wextra #-Weverything

_SRCS			=	ft_nm.c

SRCS_DIR		=	srcs
SRCS			=	$(addprefix $(SRCS_DIR)/,$(_SRCS))

OBJS_DIR		=	objs
OBJS			=	$(addprefix $(OBJS_DIR)/,$(_SRCS:%.c=%.o))

INCS_DIR		=	includes
INCS			=	-I $(INCS_DIR)
HEADER			=	$(INCS_DIR)/ft_nm.h

all: $(HEADER) $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS)
	@$(CC) $(CFLAGS) $(INCS) -o $(NAME) $(OBJS)
	@echo "$(NAME) : Done"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(HEADER)
	@$(CC) $(CFLAGS) $(INCS) -c -o $@ $<

clean:
	@/bin/rm -rf $(OBJS_DIR)
	@echo "$(NAME) clean : Done"

fclean: clean
	@/bin/rm -f $(NAME)
	@echo "$(NAME) fclean : Done"

re:
	@make fclean
	@make

.PHONY: all clean fclean re
