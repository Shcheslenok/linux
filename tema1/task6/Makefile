SOURCES=client.c server.c

all: main

main: $(SOURCES)
	gcc client.c -o client
	gcc server.c -lpthread -o server

clean:
	rm -f client server
