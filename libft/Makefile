# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/15 17:04:31 by bsuc              #+#    #+#              #
#    Updated: 2024/03/23 17:10:03 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OBJ_DIR = obj

SRCS = \
	ft_split.c \
	ft_memset.c \
	ft_strchr.c \
	ft_substr.c \
	ft_strlen.c \
	ft_bzero.c \
	ft_calloc.c \
	ft_strncmp.c \
	ft_strnstr.c \
	ft_strtrim.c \
	ft_isprint.c \
	ft_strdup.c \
	ft_itoa.c \
	ft_atoi.c \
	ft_putstr_fd.c \
	ft_isdigit.c \
	ft_isalnum.c \
	ft_isalpha.c

OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))
NAME = libft.a
RM = rm -rf
CC = cc
CFLAGS = -Wall -Wextra -Werror

all : ${NAME}

${NAME} : $(OBJ_DIR) ${OBJS}
	ar rc ${NAME} ${OBJS}

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

clean :
	${RM} $(OBJ_DIR)

fclean : clean
	${RM} ${NAME}

re : fclean all

bonus : ${OBJS} 
	ar rc ${NAME} ${OBJS}
