# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/21 11:08:49 by adi-stef          #+#    #+#              #
#    Updated: 2023/08/18 15:05:42 by adi-stef         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ircserv
NAME_BOT	= ircbot
# files
SRV_SRC		= server/Server.cpp server/User.cpp server/Channel.cpp \
				server/Commands.cpp server/Utils.cpp server/main.cpp
SRV_OBJ		= $(SRV_SRC:%.cpp=%.o)
BOT_SRC		= bot/Bot.cpp bot/Game.cpp bot/main.cpp server/Utils.cpp
BOT_OBJ		= $(BOT_SRC:%.cpp=%.o)

CC		= c++
FLAGS	= -Wall -Wextra -Werror -std=c++98
RM		= rm -f
# colors
RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
RESET	= \033[0;0m

$(shell ./ipconfig.sh)

%.o : %.cpp
	@printf "\r\033[K$(GREEN)Compiling...$(RESET)"
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(SRV_OBJ)
	@$(CC) $(FLAGS) $(SRV_OBJ) -o $(NAME)
	@printf "\r\033[K$(GREEN)$(NAME) compiled successfully$(RESET)\n"

$(NAME_BOT): $(BOT_OBJ)
	@$(CC) $(FLAGS) $(BOT_OBJ) -o $(NAME_BOT)
	@printf "\r\033[K$(GREEN)$(NAME_BOT) compiled successfully$(RESET)\n"

all: $(NAME)

bonus: $(NAME) $(NAME_BOT)

clean:
	@$(RM) $(SRV_OBJ)
	@$(RM) $(BOT_OBJ)
	@printf "$(RED)object files removed$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@$(RM) $(NAME_BOT)
	@printf "$(RED)executable removed$(RESET)\n"

re: fclean all bonus

.PHONY: all clean fclean re
