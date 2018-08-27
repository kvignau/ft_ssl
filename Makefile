# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kvignau <kvignau@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/01/04 10:52:32 by kvignau           #+#    #+#              #
#    Updated: 2016/03/23 19:49:01 by kvignau          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ssl

C_DIR = srcs/
O_DIR = obj/

FT_PRINTF = ft_printf/

CC = gcc
FLAGS = -Wall -Wextra -Werror

C_FILES = ft_ssl.c
C_FILES += ft_sha256.c
C_FILES += ft_md5.c

O_FILES = $(addprefix $(O_DIR),$(C_FILES:.c=.o))

all: ftprintf $(NAME)

ftprintf:
	make -C $(FT_PRINTF)

$(NAME): $(O_FILES)
	$(CC) $(FLAGS) -o $(NAME) $(O_FILES) -L $(FT_PRINTF) -lftprintf

$(O_FILES): $(O_DIR)%.o: $(C_DIR)%.c
	@mkdir $(O_DIR) 2> /dev/null || echo "" > /dev/null
	$(CC) $(FLAGS) -o $@ -c $< -Iincludes

clean:
	make -C $(FT_PRINTF) clean
	@rm -rf $(O_DIR) 2> /dev/null || echo "" > /dev/null

fclean: clean
	make -C $(FT_PRINTF) fclean
	@rm -f $(NAME) 2> /dev/null || echo "" > /dev/null

re: fclean all

.PHONY: re clean fclean all