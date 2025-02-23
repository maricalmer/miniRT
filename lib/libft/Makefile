# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/21 18:33:06 by dlemaire          #+#    #+#              #
#    Updated: 2025/02/23 16:01:10 by dlemaire         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = ft_atoi \
			ft_bzero \
			ft_calloc \
			ft_isalnum \
			ft_isalpha \
			ft_isascii \
			ft_isdigit \
			ft_isprint \
			ft_iswhitespace \
			ft_itoa \
			ft_memchr \
			ft_memcmp \
			ft_memcpy \
			ft_memmove \
			ft_memset \
			ft_putchar_fd \
			ft_putendl_fd \
			ft_putnbr_fd \
			ft_putstr_fd \
			ft_split \
			ft_strchr \
			ft_strdup \
			ft_striteri \
			ft_strjoin \
			ft_strlcat \
			ft_strlcpy \
			ft_strlen \
			ft_strmapi \
			ft_strncmp \
			ft_strnstr \
			ft_strrchr \
			ft_strtrim \
			ft_strtof \
			ft_substr \
			ft_tolower \
			ft_toupper \
			ft_lstnew \
			ft_lstadd_front \
			ft_lstsize \
			ft_lstlast \
			ft_lstadd_back \
		    ft_lstdelone \
			ft_lstclear \
		    ft_lstiter \
		    ft_lstmap \
		    ft_printf \
		    ft_printf_utils \
		    ft_printf_utils2 \
			get_next_line_bonus \


SRCS		= $(addsuffix .c, ${SRC})

OBJS		= ${SRCS:.c=.o}

CC			= cc
RM			= rm -f
AR			= ar rc
RN			= ranlib

CFLAGS 		= -g -Wall -Wextra -Werror

NAME		= libft.a

.c.o:
			${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME):	${OBJS}
			${AR} ${NAME} ${OBJS}
			${RN} ${NAME}

all:		$(NAME)

clean:
			${RM} ${OBJS} ${BONUS_OBJS}

fclean:		clean
			${RM} $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
