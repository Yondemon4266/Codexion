
NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread -MMD -MP

SRC = coders

BUILD_DIR = .build

C_FILES = $(SRC)/main.c $(SRC)/parsing.c $(SRC)/init_data.c \
		$(SRC)/utils.c $(SRC)/monitor.c $(SRC)/routine.c \
		$(SRC)/fill_dongles_coders.c

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


.PHONY: all clean fclean re