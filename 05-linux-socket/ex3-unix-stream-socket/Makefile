

CUR_DIR := .

client:
	gcc $(CUR_DIR)/unix-stream-client.c -o $(CUR_DIR)/unix-stream-client

server:
	gcc $(CUR_DIR)/unix-stream-server.c -o $(CUR_DIR)/unix-stream-server

all: client server

clean:
	rm -rf $(CUR_DIR)/unix-stream-client $(CUR_DIR)/unix-stream-server
	rm -rf $(CUR_DIR)/socket_stream