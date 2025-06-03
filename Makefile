NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = src/main.c src/garbage_free.c src/sigint.c \
	src/builtins/builtins.c src/parser/parser.c src/parser/utils.c \
	src/executor/executor.c src/executor/utils.c src/tokens.c \
	src/builtins/builtin_echo.c src/builtins/builtin_pwd.c \
	src/builtins/builtin_env.c src/builtins/builtin_cd.c \
	src/builtins/builtin_export.c src/builtins/builtin_unset.c src/builtins/builtin_exit.c

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = ./libs/libft
LIBFT_A = $(LIBFT_DIR)/libft.a
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline

all: $(LIBFT_A) $(NAME)

$(LIBFT_A):
	@echo "Compilando libft..."
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

val:
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=my.supp ./minishell

.PHONY: all clean fclean re val
