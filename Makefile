# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/15 03:03:24 by hiroaki           #+#    #+#              #
#    Updated: 2022/12/10 01:42:02 by hiroaki          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	philo
BONUS_NAME		=	philo_bonus

FLAGS			=	-Wall -Wextra -Werror -I$(INC) -g -fsanitize=address -fsanitize=integer -fsanitize=undefined

MAND			=	main.c
BONUS			=	main.c
UTILS			=	ft_atoi.c \
					ft_isdigit.c

MAND_PATH		= 	$(addprefix mandatory/main/, $(MAND))
MAND_OBJS_PATH	=	$(MAND_PATH:%.c=%.o)

BONUS_PATH		= 	$(addprefix bonus/main/, $(BONUS))
BONUS_OBJS_PATH	=	$(BONUS_PATH:%.c=%.o)

UTILS_PATH		= 	$(addprefix utils/, $(UTILS))
UTILS_OBJS_PATH	=	$(UTILS_PATH:%.c=%.o)

INC_PATH		=	./include

all: 			$(NAME)

$(NAME):		$(MAND_OBJS_PATH) $(UTILS_OBJS_PATH)
				$(CC) $(FLAGS) $(MAND_OBJS_PATH) $(UTILS_OBJS_PATH) -o $(NAME)

$(BONUS_NAME):	$(BONUS_OBJS_PATH)
				$(CC) $(FLAGS) $(BONUS_OBJS_PATH) $(UTILS_OBJS_PATH) -o $(BONUS_NAME)

clean:
				$(RM) $(MAND_OBJS_PATH)
				$(RM) $(BONUS_OBJS_PATH)
				$(RM) $(UTILS_OBJS_PATH)

fclean:			clean
				$(RM) $(NAME)
				$(RM) $(BONUS_NAME)

re:				fclean all

bonus:			$(BONUS_NAME)

.PHONY:			all clean fclean re bonus
