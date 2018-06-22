.PHONY:all
all:udp_server udp_client

udp_server:udp_server.c
	gcc -o $@ $^ 

udp_client:udp_client.c
	gcc -o $@ $^ -static

.PHONY:clean
clean:
	rm -f udp_client udp_server
