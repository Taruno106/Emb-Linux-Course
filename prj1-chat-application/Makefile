# Trình biên dịch và cờ
CC = gcc
CFLAGS = -Wall -Iinc -g

# Thư mục
SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build

# File nguồn và đối tượng
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/chat.c
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/chat.o

# Tên chương trình
TARGET = $(BUILD_DIR)/chat_app

# Build tất cả
all: $(TARGET)

# Tạo chương trình
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Biên dịch từng file .c thành .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Dọn sạch
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
