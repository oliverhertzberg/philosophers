# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ohertzbe <ohertzbe@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/21 15:18:39 by ohertzbe          #+#    #+#              #
#    Updated: 2024/06/16 13:29:09 by ohertzbe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := cc
CFLAGS := -fsanitize=thread -g -Wall -Wextra -Werror
NAME := philo

HEADER := -I ./include
SRC := src/main.c src/utils.c src/cleaning.c src/create_mutexes.c \
	src/get_time.c src/init_philos.c src/philo_functions.c \
	src/supervisor.c src/write_state.c
OBJ := $(patsubst src/%.c, obj/%.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADER)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: clean all

.PHONY: all clean fclean re	