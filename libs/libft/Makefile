NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -fPIC -g

SRC = ./ft_isascii.c \
	  ./ft_isalpha.c \
	  ./ft_toupper.c \
	  ./ft_tolower.c \
	  ./ft_isalnum.c \
	  ./ft_isprint.c \
	  ./ft_isdigit.c \
	  ./ft_atoi.c \
	  ./ft_strncmp.c \
	  ./ft_memcmp.c \
	  ./ft_strdup.c \
	  ./ft_strchr.c \
	  ./ft_strrchr.c \
	  ./ft_strnstr.c \
	  ./ft_strcmp.c \
	  ./ft_bzero.c \
	  ./ft_calloc.c \
	  ./ft_memset.c \
	  ./ft_memchr.c \
	  ./ft_memcpy.c \
	  ./ft_memmove.c \
	  ./ft_strlen.c \
	  ./ft_strcpy.c \
	  ./ft_strlcpy.c \
	  ./ft_substr.c \
	  ./ft_strjoin.c \
	  ./ft_strtrim.c \
	  ./ft_putchar_fd.c \
	  ./ft_putstr_fd.c \
	  ./ft_putnbr_fd.c \
	  ./ft_putendl_fd.c \
	  ./ft_striteri.c \
	  ./ft_itoa.c \
	  ./ft_strmapi.c \
	  ./ft_split.c \
	  ./ft_strndup.c \
	  ./ft_strtok.c \
	  ./ft_strlcat.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
