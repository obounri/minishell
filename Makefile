NAME = minishell
CC = gcc
# CFLAGS = -Wall -Wextra -Werror
HEADER = includes/minishell.h

UTILS = utils/*.c
# remove wildcard 

LIBFT_MAKEFILE_PATH = utils/libft/ 
LIBFT = utils/libft/libft.a
# LIBFT_SRCS = srcs/libft/ft_atoi.c

all: $(NAME)

$(NAME): $(HEADER) main.c $(UTILS) $(LIBFT)
	$(CC) $(CFLAGS) main.c $(UTILS) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_MAKEFILE_PATH)

clean:
	make clean -C $(LIBFT_MAKEFILE_PATH)

fclean: clean
	rm -f $(NAME) $(LIBFT)

re: fclean all