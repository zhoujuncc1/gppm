CC=nvcc
MAKE=make
CPFLAGS= -g  -std=c++11 -Iinclude 

all: libblstl.a libmining.a

libblstl.a:
	$(MAKE) -C Bltl all
	ar rcs libblstl.a Bltl/*.o

libmining.a:
	$(MAKE) -C Mining all
	ar rcs libmining.a Mining/*.o

clean:
	$(MAKE) -C Mining clean
	$(MAKE) -C Bltl clean
	rm -f *.a

