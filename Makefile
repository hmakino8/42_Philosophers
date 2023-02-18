# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/15 03:03:24 by hiroaki           #+#    #+#              #
#    Updated: 2023/02/18 22:09:42 by hiroaki          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	philo
INCLUDE =	./include
CFLAGS	=	-Wall -Wextra -Werror -I$(INCLUDE)

MAND	=	main.c \
			init.c \
			put.c \
			helper.c \
			exit.c

UTILS	=	ft_atoi.c

SRCS	=	$(addprefix mandatory/, $(MAND)) \
			$(addprefix utils/, $(UTILS))
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
