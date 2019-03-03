CC=nvcc
MAKE=make
CPFLAGS= -g  -std=c++11 -Iinclude 

all: libbltl.a libmining.a

libbltl.a:
	$(MAKE) -C Bltl all
	ar rcs libbltl.a Bltl/*.o

libmining.a:
	$(MAKE) -C Mining all
	ar rcs libmining.a Mining/*.o

clean:
	$(MAKE) -C Mining clean
	$(MAKE) -C Bltl clean
	rm -f *.a

