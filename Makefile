# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: psaulnie <psaulnie@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/18 14:38:40 by psaulnie          #+#    #+#              #
#    Updated: 2023/03/06 13:28:15 by psaulnie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
NAME_BONUS = theMysteryMachine

# **************************************************************************** #
#                               FILES and PATHS                                #
# **************************************************************************** #

SRCS =	$(shell find srcs -name "*.cpp")
HEADFILE = $(shell find incs -name "*.hpp")
DIR_OBJ = srcs/.objs

SRCS_BONUS =	$(shell find bonus/srcs -name "*.cpp")
HEADFILE_BONUS = $(shell find bonus/incs -name "*.hpp")
DIR_OBJ_BONUS = bonus/srcs/.objs_bonus

CFLAGS = -Wall -Wextra -Werror
CFLAGS += -std=c++98
#CFLAGS += -fsanitize=address -g3

# **************************************************************************** #
#                                    COLORS                                    #
# **************************************************************************** #

BLACK			=	\033[0;30m
RED				=	\033[0;31m
GREEN			=	\033[0;32m
ORANGE			=	\033[0;33m
BLUE			=	\033[0;34m
PURPLE			=	\033[0;35m
CYAN			=	\033[0;36m
LIGHT_GRAY		=	\033[0;37m
DARK_GRAY		=	\033[1;30m
LIGHT_RED		=	\033[1;31m
LIGHT_GREEN		=	\033[1;32m
YELLOW			=	\033[1;33m
LIGHT_BLUE		=	\033[1;34m
LIGHT_PURPLE	=	\033[1;35m
LIGHT_CYAN		=	\033[1;36m
WHITE			=	\033[1;37m
NO_COLOR		=	\033[0m

# **************************************************************************** #
#                              COMMANDS and RULES                              #
# **************************************************************************** #

.SILENT:

.DEFAULT_GOAL = all

OBJS = $(patsubst srcs/%.cpp, $(DIR_OBJ)/%.o, $(SRCS))
OBJS_BONUS = $(patsubst bonus/srcs/%.cpp, $(DIR_OBJ_BONUS)/%.o, $(SRCS_BONUS))

all: $(NAME)

bonus: $(NAME_BONUS)

$(DIR_OBJ)/%.o: srcs/%.cpp
	mkdir -p $(shell dirname $@)
	c++ $(CFLAGS) -o $@ -c $<
	@printf "\r$(LIGHT_GRAY)Loading...$(NO_COLOR)"

$(DIR_OBJ_BONUS)/%.o: bonus/srcs/%.cpp
	mkdir -p $(shell dirname $@)
	c++ $(CFLAGS) -o $@ -c $<
	@printf "\r$(LIGHT_GRAY)Loading bonus...$(NO_COLOR)"

$(NAME): $(OBJS) $(HEADFILE)
	c++ $(CFLAGS) -o $(NAME) $(SRCS)
	@printf "\r$(LIGHT_GREEN)➞$(NO_COLOR) Compiled $(LIGHT_GREEN)✔$(NO_COLOR)\n"

$(NAME_BONUS): $(OBJS_BONUS) $(HEADFILE_BONUS)
	c++ $(CFLAGS) -o $(NAME_BONUS) $(SRCS_BONUS)
	@printf "\r$(LIGHT_GREEN)➞$(NO_COLOR) Compiled with bonus $(LIGHT_GREEN)✔$(NO_COLOR)\n"

$(DIR_OBJ):
	mkdir -p $(DIR_OBJ)

$(DIR_OBJ_BONUS):
	mkdir -p $(DIR_OBJ_BONUS)

clean:
	rm -rf $(OBJS)
	rm -rf $(OBJS_BONUS)
	rm -rf $(DIR_OBJ) $(DIR_OBJ_BONUS)
	@printf "\r$(LIGHT_BLUE)➞$(NO_COLOR) Cleaned $(LIGHT_BLUE)✔$(NO_COLOR)\n"

fclean: clean
	rm -rf $(NAME)
	rm -rf $(NAME_BONUS)

re: fclean all

.PHONY: all bonus clean fclean re
