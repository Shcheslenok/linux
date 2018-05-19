SOURCES=client.c server.c

all: client server

main: $(SOURCES)
	gcc client.c -o client
	gcc server.c -o server

clean:
	rm -f client server
