LD_LIB=libhello
OBJECTS=libgoodbye.o hello.o
EXECFILE=hello

all: $(EXECFILE)

$(EXECFILE): libs
	gcc -o $(EXECFILE) hello.o -L. -Bstatic -lgoodbye -Bdynamic -lhello

libs: $(OBJECTS) $(LD_LIB).c
	ar rcs libgoodbye.a $(OBJECTS)
	rm -rf *.so*
	gcc -fPIC -c $(LD_LIB).c -o $(LD_LIB).o
	gcc -shared -Wl,-soname,$(LD_LIB).so.0 -o $(LD_LIB).so.0.0 $(LD_LIB).o
	ln -s $(LD_LIB).so.0.0 $(LD_LIB).so
	ln -s $(LD_LIB).so.0.0 $(LD_LIB).so.0

clean:
	rm -rf *.o *.so* *.a $(EXECFILE)
