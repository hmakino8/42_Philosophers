# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/15 03:03:24 by hiroaki           #+#    #+#              #
#    Updated: 2023/02/22 18:28:40 by hmakino          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	philo

INCLUDE =	./include
CFLAGS	=	-I$(INCLUDE) -Wall -Wextra -Werror

MAND	=	main.c \
			init.c \
			routine.c \
			action.c \
			put.c \
			terminate.c \
			helper.c

UTILS	=	ft_atoi.c \
			ft_strlen.c

SRCS	=	$(addprefix src/philo/, $(MAND)) \
			$(addprefix src/utils/, $(UTILS))
OBJS	=	$(SRCS:%.c=%.o)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

all:	$(NAME)

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
