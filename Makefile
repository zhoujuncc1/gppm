NVCC=nvcc
CC=g++
NVCCFLAGS= -g  -std=c++11 -Iinclude -lgsl -lgslcblas -lm
#-lgslcblas
all: fileModel gpuModel modelChecker

fileModel: examples/fileApplication.cpp libs
	$(CC) $(NVCCFLAGS) -o fileModel examples/fileApplication.cpp src/libblstl.a src/libmining.a 
gpuModel: examples/gpuApplication.cpp libs
	$(NVCC) $(NVCCFLAGS) -o gpuModel examples/gpuApplication.cpp src/libblstl.a src/libmining.a 
modelChecker: examples/modelChecker.cpp libs
	$(CC) $(NVCCFLAGS) -o modelModel examples/modelChecker.cpp src/libblstl.a src/libmining.a 
libs:
	make -C src


clean:
	$(MAKE) -C src clean
	rm -f fileModel gpuModel modelChecker

