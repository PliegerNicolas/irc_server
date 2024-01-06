# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nicolas <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/17 21:31:44 by nicolas           #+#    #+#              #
#    Updated: 2024/01/06 13:08:14 by nicolas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#* ************************************************************************** *#
#* *                            GENERAL INFO                                * *#
#* ************************************************************************** *#

NAME					=			irc_server

#* ************************************************************************** *#
#* *                             COMPILATION                                * *#
#* ************************************************************************** *#

CC						=			c++
CC_FLAGS				=			-std=c++11 -pedantic
LIBRARY_FLAGS			=


#* ************************************************************************** *#
#* *                               SOURCES                                  * *#
#* ************************************************************************** *#

SOURCES_EXTENSION		=			.cpp
SOURCES_PATH			=			srcs

SOURCES_NAMES			:=			main \
									sockets/Sockets \
									sockets/SocketInfo \
									threadpool/ThreadPool \
									server/Server \

SOURCES					:=			$(addsuffix $(SOURCES_EXTENSION), $(SOURCES_NAMES))

#* ************************************************************************** *#
#* *                              INCLUDES                                  * *#
#* ************************************************************************** *#

INCLUDES_DIRS			=			includes
INCLUDES_FLAGS			:=			$(addprefix -I, $(INCLUDES_DIRS))

#* ************************************************************************** *#
#* *                              OBJECTS                                   * *#
#* ************************************************************************** *#

OBJECTS_PATH			=			objects

OBJECTS					:=			$(addprefix $(OBJECTS_PATH)/, $(SOURCES:.cpp=.o))

#* ************************************************************************** *#
#* *                            DEPENDENCIES                                * *#
#* ************************************************************************** *#

DEPENDENCIES			:=			$(OBJECTS:.o=.d)

#* ************************************************************************** *#
#* *                           RULES FILTER                                 * *#
#* ************************************************************************** *#

ifeq (noflag, $(filter noflag,$(MAKECMDGOALS)))
	CC_FLAGS			+=			-Wall -Wextra
else
	CC_FLAGS			+=			-Wall -Wextra -Werror
endif

ifeq (debug, $(filter debug,$(MAKECMDGOALS)))
	CC_FLAGS			+=			-g3
endif

ifeq (sanadd, $(filter sanadd,$(MAKECMDGOALS)))
	CC_FLAGS			+=			-fsanitize=address -g3
endif

ifeq (santhread, $(filter santhread,$(MAKECMDGOALS)))
	CC_FLAGS			+=			-fsanitize=thread -g3
endif

ifeq (optimize, $(filter optimize,$(MAKECMDGOALS)))
	CC_FLAGS			+=			-O3
endif

#* ************************************************************************** *#
#* *                          TEXT CONSTANTS                                * *#
#* ************************************************************************** *#

BLACK					=			\033[30m
RED						=			\033[31m
GREEN					=			\033[32m
YELLOW					=			\033[33m
BLUE					=			\033[34m
MAGENTA					=			\033[35m
CYAN					=			\033[36m

BOLD					=			\033[1m
THIN					=			\033[2m
ITALIC					=			\033[3m
UNDERLINE				=			\033[4m

RESET_COLOR				=			\033[0m

#* ************************************************************************** *#
#* *                           PRESENTATION                                 * *#
#* ************************************************************************** *#

define success_message
	@echo "✨ $(GREEN)$(BOLD)COMPILATION SUCCESSFUL$(RESET_COLOR) ✨"
endef

define linking_message
	@echo "$(YELLOW)Linking $(BOLD)$(CYAN)$@ $(RESET_TEXT)$(YELLOW)...$(RESET_COLOR)"
endef

LAST_DIR = ""
FIRST_COMPILE_MESSAGE = true
define compile_message
	@if [ "$(FIRST_COMPILE_MESSAGE)" = "true" ]; then \
		FIRST_COMPILE_MESSAGE=false; \
	fi
	@if [ "$(dir $<)" != "$(LAST_DIR)" ]; then \
		printf "$(BOLD)$(YELLOW)Compiling files in directory $(RESET_COLOR)$(BOLD)$(CYAN)$(dir $<)$(RESET_COLOR)\n"; \
		LAST_DIR="$(dir $<)"; \
	fi
	printf "$(CYAN)    • $(notdir $<)\n";
	@$(eval LAST_DIR := $(dir $<))
endef

#* ************************************************************************** *#
#* *                          MAKEFILE RULES                                * *#
#* ************************************************************************** *#

all:					$(NAME)

# -------------------- #
# Create object files. #
# -------------------- #

$(OBJECTS_PATH)/%.o:	$(SOURCES_PATH)/%$(SOURCES_EXTENSION)
	@mkdir -p $(dir $@)
	@$(call compile_message)
	@$(CC) $(CC_FLAGS) -MMD -MF $(@:.o=.d)  $(INCLUDES_FLAGS) -c $< -o $@

# ---------------------------------------- #
# Link the files after compiling them all. #
# ---------------------------------------- #

-include $(DEPENDENCIES)
$(NAME):				$(OBJECTS)
	@echo ""
	@$(call linking_message)
	@$(CC) $(CC_FLAGS) $(INCLUDES_FLAGS) -o $@ $(OBJECTS) $(LIBRARY_FLAGS)
	@echo ""
	@$(call success_message)

# --------------------- #
# Delete compiled data. #
# --------------------- #

clean:
	@echo "$(YELLOW)Deleting $(CYAN)$(OBJECTS_PATH) $(YELLOW)...$(RESET_TEXT)"
	@rm -rf $(OBJECTS_PATH)

fclean:					clean
	@echo "$(YELLOW)Deleting $(CYAN)$(NAME) executable $(YELLOW)...$(RESET_TEXT)"
	@rm -f $(NAME)

# --------------------- #
# Recompile.			#
# --------------------- #

re:						clean all

# --------------------- #
# Additional flag.		#
# --------------------- #

noflag: 				all

debug:					all

sanadd:					all

santhread:				all

optimize:				all

.PHONY:	header clean fclean re run noflag debug
