all: server.c client.c
	gcc -Wall -o server server.c -lm
	gcc -Wall -o client client.c
clean:
	rm -rf server client

.PHONY: all clean