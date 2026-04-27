
NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -g -pthread -MMD -MP

SRC = coders

INIT = coders/init

SIMULATION = $(SRC)/simulation

SCHEDULER = $(SRC)/scheduler

UTILS = $(SRC)/utils

BUILD_DIR = .build

C_FILES = $(SRC)/main.c \
		$(INIT)/fill_dongles_coders.c $(INIT)/parse_data.c $(INIT)/init_data.c \
		$(SIMULATION)/monitor.c $(SIMULATION)/monitor_tracking.c \
		$(SIMULATION)/coder_routine.c $(SIMULATION)/coder_actions.c \
		$(SCHEDULER)/queue_manager.c $(SCHEDULER)/dongle_actions.c \
		$(UTILS)/cleanup.c $(UTILS)/status.c $(UTILS)/time.c \
		$(UTILS)/print.c

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