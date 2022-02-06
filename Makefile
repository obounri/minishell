NAME = minishell

CC = gcc

CFLAGS = -Wall -Werror -Wextra

SRC_FILE = main.c \
utils/check_errors.c \
utils/expand.c \
utils/implemented.c \
utils/init.c \
utils/redirect.c \
utils/redirect_1.c \
utils/redirect_2.c \
utils/redirect_3.c \
utils/unset.c \
utils/quotes.c \
utils/quotes_utils.c\
utils/export.c \
utils/export_print.c \
utils/parsing.c \
utils/execution.c \
utils/clean.c \
utils/error_utils.c
OBJ_FILE = $(SRC_FILE:.c=.o)

LIBFT_MAKEFILE_PATH = utils/libft/ 
LIBFT = utils/libft/libft.a


all : $(NAME)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
# -I$(shell brew --prefix readline)/include

$(NAME) : $(OBJ_FILE) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ_FILE) $(LIBFT) -lreadline -o $(NAME)
# -L$(shell brew --prefix readline)/lib

$(LIBFT):
	make -C $(LIBFT_MAKEFILE_PATH)

clean :
	rm -rf $(OBJ_FILE) 
	make clean -C $(LIBFT_MAKEFILE_PATH)

fclean : clean
	rm -rf $(NAME) $(LIBFT)

re : fclean all
