

CUR_DIR := .

client:
	gcc $(CUR_DIR)/ipv4-dgram-client.c -o $(CUR_DIR)/ipv4-dgram-client

server:
	gcc $(CUR_DIR)/ipv4-dgram-server.c -o $(CUR_DIR)/ipv4-dgram-server

all: client server

clean:
	rm -rf $(CUR_DIR)/ipv4-dgram-client $(CUR_DIR)/ipv4-dgram-server
