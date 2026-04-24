
NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -std=c89 -g -D_XOPEN_SOURCE=500 -pthread -MMD -MP

SRC = coders

BUILD_DIR = .build

C_FILES = $(SRC)/main.c $(SRC)/parse_data.c $(SRC)/init_data.c \
		$(SRC)/utils.c $(SRC)/fill_dongles_coders.c $(SRC)/cleanup.c \
		$(SRC)/monitor.c $(SRC)/monitor_tracking.c $(SRC)/coder_routine.c

O_FILES = $(C_FILES:%.c=$(BUILD_DIR)/%.o)

INCLUDES = $(SRC)/data.h

DEPS = $(O_FILES:.o=.d)

$(NAME): $(O_FILES)
	$(CC) $(CFLAGS) $(O_FILES) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)
fclean: clean
	rm -rf $(NAME)

re: fclean all

chleak: 
	valgrind --leak-check=full --show-leak-kinds=all ./codexion 4 800 200 200 200 5 100 fifo
	

.PHONY: all clean fclean re