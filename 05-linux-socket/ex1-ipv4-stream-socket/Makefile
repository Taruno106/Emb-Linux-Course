

CUR_DIR := .

client:
	gcc $(CUR_DIR)/ipv4-stream-client.c -o $(CUR_DIR)/ipv4-stream-client

server:
	gcc $(CUR_DIR)/ipv4-stream-server.c -o $(CUR_DIR)/ipv4-stream-server

all: client server

clean:
	rm -rf $(CUR_DIR)/ipv4-stream-client $(CUR_DIR)/ipv4-stream-server
