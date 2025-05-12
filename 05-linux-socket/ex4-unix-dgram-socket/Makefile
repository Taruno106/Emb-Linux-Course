

CUR_DIR := .

client:
	gcc $(CUR_DIR)/unix-dgram-client.c -o $(CUR_DIR)/unix-dgram-client

server:
	gcc $(CUR_DIR)/unix-dgram-server.c -o $(CUR_DIR)/unix-dgram-server

all: client server

clean:
	rm -rf $(CUR_DIR)/unix-dgram-client $(CUR_DIR)/unix-dgram-server
	rm -rf $(CUR_DIR)/dgram_server $(CUR_DIR)/dgram_client