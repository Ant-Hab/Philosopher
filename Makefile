# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/17 16:13:38 by achowdhu          #+#    #+#              #
#    Updated: 2025/08/19 13:13:02 by achowdhu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ========================================
#           Project Information
# ========================================

NAME        := philo

# Compiler and Flags
CC          := cc
CFLAGS      := -Wall -Wextra -Werror
THREAD_LIB  := -pthread

# Directories
SRC_DIR     := src
OBJ_DIR     := obj
INC_DIR     := inc

# Source and Object Files
SRCS        := main.c init.c philo.c monitor.c utils.c
OBJS        := $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))

# Includes
INC         := -I$(INC_DIR)

# ========================================
#               Colors
# ========================================

GREEN       := \033[0;32m
YELLOW      := \033[0;33m
RESET       := \033[0m

# ========================================
#               Rules
# ========================================

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)[Linking]$(RESET) $(NAME)"
	@$(CC) $(CFLAGS) $(OBJS) $(THREAD_LIB) -o $(NAME)
	@echo "$(GREEN)✔ $(NAME) built successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(GREEN)[Compiling]$(RESET) $<"
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "$(YELLOW)[Cleaning]$(RESET) object files"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(YELLOW)[Removing]$(RESET) $(NAME)"
	@rm -f $(NAME)
	@echo "$(GREEN)✔ All files removed successfully!$(RESET)"

re: fclean all

.SECONDARY: $(OBJS)
.PHONY: all clean fclean re
