LIBRARIES=libhello.c libgoodbye.c
MAIN=hello.c
OBJECTS=$(LIBRARIES:.c=.o)
EXECFILE=hello

all: $(EXECFILE)

$(EXECFILE): libs
	gcc -o $(EXECFILE) hello.c -L. -lmylib

libs: $(OBJECTS)
	ar rcs libmylib.a $(OBJECTS)

clean:
	rm -rf *.o *.a $(EXECFILE)
