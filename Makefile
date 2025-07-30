CC = gcc
CFLAGS = -O2 -Wall `pkg-config --cflags gtk+-3.0 gtk-layer-shell-0`
LDFLAGS = `pkg-config --libs gtk+-3.0 gtk-layer-shell-0` -lm -lX11 -lXext

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))
EXEC = $(BIN_DIR)/overlay_x11

.PHONY: all clean fclean re

all: $(EXEC)

$(EXEC): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(wildcard $(SRC_DIR)/*.h)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(EXEC)

re: fclean all
