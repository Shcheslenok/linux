SOURCES=proc_A.c proc_B.c proc_C.c main.c

all: main

main: $(SOURCES)
	gcc proc_A.c -o proc_A
	gcc proc_B.c -o proc_B
	gcc proc_C.c -o proc_C
	gcc main.c -o main

clean:
	rm -f proc_A proc_B proc_C main
