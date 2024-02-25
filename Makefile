# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/29 23:25:19 by bsuc              #+#    #+#              #
#    Updated: 2024/02/25 16:45:25 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_DIR = src
PARS_DIR = $(SRC_DIR)/parsing
EXEC_DIR = $(SRC_DIR)/execution
BUIL_DIR = $(SRC_DIR)/builtins

SRC_MAIN = $(SRC_DIR)/minishell.c \
	$(SRC_DIR)/copy_envp.c \
	$(SRC_DIR)/sanitize.c \
	$(SRC_DIR)/print.c # to remove

SRC_PARS = $(PARS_DIR)/check_syntax.c \
	$(PARS_DIR)/fill_cmd.c \
	$(PARS_DIR)/fill_redir.c \
	$(PARS_DIR)/fill_struct.c \
	$(PARS_DIR)/manage_list.c \
	$(PARS_DIR)/parsing_utils.c \
	$(PARS_DIR)/parsing.c

SRC_BUIL = $(BUIL_DIR)/builtins.c \
	$(BUIL_DIR)/cd.c \
	$(BUIL_DIR)/echo.c \
	$(BUIL_DIR)/env.c \
	$(BUIL_DIR)/export.c \
	$(BUIL_DIR)/unset.c

SRC_EXEC = $(EXEC_DIR)/checks_comms.c \
	$(EXEC_DIR)/errors.c \
	$(EXEC_DIR)/execute.c \
	$(EXEC_DIR)/handle_var_utils.c \
	$(EXEC_DIR)/handle_var.c \
	$(EXEC_DIR)/heredoc.c \
	$(EXEC_DIR)/pipes.c \
	$(EXEC_DIR)/quote_removing.c \
	$(EXEC_DIR)/redirections.c \
	$(EXEC_DIR)/signals.c

SRC = $(SRC_MAIN) \
	$(SRC_PARS) \
	$(SRC_BUIL) \
	$(SRC_EXEC)

OBJ = $(SRC:.c=.o)
NAME = minishell
RM = rm -f
CC = cc
CFLAGS = -Wall -Wextra  -g #-Werror
LIBFT = libft/libft.a
INC = -I./headers

all : $(NAME)

$(NAME) : $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIBFT) :
	make -C libft all

clean :
	make -C libft clean
	$(RM) $(OBJ)

fclean : clean
	make -C libft fclean
	$(RM) $(NAME)

re : fclean all

val : all
	valgrind --suppressions=readline.supp --track-fds=yes --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell
