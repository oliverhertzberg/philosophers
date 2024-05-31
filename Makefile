# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/21 15:18:39 by ohertzbe          #+#    #+#              #
#    Updated: 2024/05/31 16:26:38 by ohertzbe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := cc
CFLAGS := -g -Wall -Wextra -Werror
NAME := philo

HEADER := -I ./philo.h
SRC := src/main.c src/ft_atoi.c src/utils.c
OBJ := $(patsubst src/%.c, obj/%.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(OBJ) $(HEADER) -o $(NAME)

obj/%.o: src/%.c
	@mkdir -p obj
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADER)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: clean all

.PHONY: all, clean, fclean, re	