CC=g++
MAKE=make
CPFLAGS= -O3 -std=c++11 -I../include 

all: libbltl.a libmining.a
	
gpu: libbltl_gpu.a libmining_gpu.a

libbltl.a:
	$(MAKE) -C Bltl all
	ar rcs libbltl.a Bltl/*.o

libmining.a: 	
	$(MAKE) -C Mining all
	ar rcs libmining.a Mining/miner_utils.o Mining/ParameterSet.o Mining/IntervalSA.o Mining/Loss.o

libbltl_gpu.a:
	$(MAKE) -C Bltl gpu
	ar rcs libbltl_gpu.a Bltl/BltlChecker.o

libmining_gpu.a:
	$(MAKE) -C Mining gpu
	ar rcs libmining_gpu.a Mining/GPUModel.o
clean:
	$(MAKE) -C Mining clean
	$(MAKE) -C Bltl clean
	rm -f *.a

