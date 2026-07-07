# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/13 11:16:05 by rcompain          #+#    #+#              #
#    Updated: 2026/07/07 17:48:05 by rcompain         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ircserv
CC			= c++
CFLAGS		= -Wall -Wextra -Werror -std=c++98 -g
RM			= rm -f

# ——— Dossiers ————————————————————————————————————————————————————————————— #
SRC_DIR		= src
OBJ_DIR		= obj

# --- Sous-dossiers sources --------------------------------------------------
SUB_DIRS	:= server clients channels server/commands reply parsing

VPATH		:= $(SRC_DIR) \
			   $(addprefix $(SRC_DIR)/, $(SUB_DIRS))

# ——— Sources ——————————————————————————————————————————————————————————————— #
SRC_SERVER	= Server.cpp

SRC_CLIENTS	= Client.cpp

SRC_CHANNELS = Channel.cpp

SRC_COMMANDS = commandCheck.cpp commandHandler.cpp invite.cpp join.cpp kick.cpp mode.cpp nick.cpp part.cpp pass.cpp ping.cpp privmsg.cpp quit.cpp topic.cpp user.cpp

SRC_REPLY	 = Reply.cpp

SRC_PARSING	 = message.cpp


SRCS		= main.cpp $(SRC_SERVER) $(SRC_CLIENTS) $(SRC_CHANNELS) $(SRC_COMMANDS) $(SRC_REPLY) $(SRC_PARSING)

OBJ			= ${SRCS:%.cpp=$(OBJ_DIR)/%.o}

# ——— Includes ——————————————————————————————————————————————————————————————— #
CPPFLAGS	:= -I$(SRC_DIR) $(addprefix -I$(SRC_DIR)/, $(SUB_DIRS))

# ——— Colors ———————————————————————————————————————————————————————————————— #
R           = \033[0m
BOLD        = \033[22m\033[1m
DIM         = \033[2m
GREEN       = \033[32m
CYAN        = \033[36m
ORANGE      = \033[38;5;183m
RED         = \033[38;5;210m

# ——— Banner ———————————————————————————————————————————————————————————————— #
define BANNER
$(BOLD)$(ORANGE)\n  ██$(DIM)╗██████$(DIM)╗  ██████$(DIM)╗\n  ██$(DIM)║██$(DIM)╔══██$(DIM)╗██$(DIM)╔════╝\n  ██$(DIM)║██████$(DIM)╔╝██$(DIM)║\n  ██$(DIM)║██$(DIM)╔══██$(DIM)╗██$(DIM)║\n  ██$(DIM)║██║  ██$(DIM)║╚██████$(DIM)╗\n  ╚═╝╚═╝  ╚═╝ ╚═════╝$(R)\n$(DIM)                    by rcompain — 42 Angoulême\n\n$(R)
endef
export BANNER

# ——— Rules ————————————————————————————————————————————————————————————————— #
all: banner $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@printf "\r\033[2K$(CYAN)📝 Sources     $(BOLD)$(GREEN)[OK]$(R)\n"
	@printf "$(BOLD)$(GREEN)\n    ✅  $(NAME) compiled successfully\n\n$(R)"

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	@printf "\r\033[2K$(CYAN)📝 Compiling   %s$(R)" "$<"

banner:
	@printf "$$BANNER"

# ——— Cleanup ——————————————————————————————————————————————————————————————— #
clean:
	@$(RM) -r $(OBJ_DIR)
	@printf "$(CYAN)🗑  Object files removed$(R)\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(CYAN)🗑  Executable removed$(R)\n"

re: fclean all

.PHONY: all clean fclean re banner
