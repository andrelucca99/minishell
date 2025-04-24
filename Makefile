# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eschula <eschula@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/23 00:00:00 by emerson           #+#    #+#              #
#    Updated: 2025/04/23 17:45:21 by eschula          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./include
LDFLAGS = -lreadline

# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Diretórios dos módulos
LEXER_DIR = $(SRC_DIR)/lexer
PARSER_DIR = $(SRC_DIR)/parser
EXPANDER_DIR = $(SRC_DIR)/expander
EXECUTOR_DIR = $(SRC_DIR)/executor
BUILTINS_DIR = $(SRC_DIR)/builtins
SIGNALS_DIR = $(SRC_DIR)/signals

# Arquivos fonte
SRC_FILES = $(SRC_DIR)/main.c

# Fontes dos módulos (a serem adicionados à medida que forem implementados)
#LEXER_SRC = $(LEXER_DIR)/tokenize.c
#PARSER_SRC = $(PARSER_DIR)/parse.c
#EXPANDER_SRC = $(EXPANDER_DIR)/expand.c
#EXECUTOR_SRC = $(EXECUTOR_DIR)/execute.c
#BUILTINS_SRC = $(BUILTINS_DIR)/builtins.c
#SIGNALS_SRC = $(SIGNALS_DIR)/signals.c

# Juntar todos os arquivos fonte
SRCS = $(SRC_FILES)

# Arquivos objeto
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Regras
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Compilação concluída! Execute './$(NAME)' para iniciar o shell."

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/lexer
	@mkdir -p $(OBJ_DIR)/parser
	@mkdir -p $(OBJ_DIR)/expander
	@mkdir -p $(OBJ_DIR)/executor
	@mkdir -p $(OBJ_DIR)/builtins
	@mkdir -p $(OBJ_DIR)/signals

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re