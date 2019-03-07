NVCC=nvcc
CC=g++
NVCCFLAGS=-g -std=c++11 -Iinclude -lgsl -lgslcblas -lm -lboost_system -lboost_thread
#-lgslcblas
all: fileModel modelChecker scoreServer#gpuModel gpuChecker

fileModel: examples/fileApplication.cpp libs
	$(CC) examples/fileApplication.cpp src/libbltl.a src/libmining.a $(NVCCFLAGS) -o fileModel
gpuModel: examples/gpuApplication.cpp libs
	$(NVCC) examples/gpuApplication.cpp src/libbltl.a src/libmining.a $(NVCCFLAGS) -o gpuModel 
modelChecker: examples/modelChecker.cpp libs
	$(CC) examples/modelChecker.cpp src/libbltl.a src/libmining.a $(NVCCFLAGS) -o modelChecker
gpuChecker: examples/gpuChecker.cpp libs
	$(NVCC) examples/gpuChecker.cpp src/libbltl.a src/libmining.a $(NVCCFLAGS) -o gpuChecker

scoreServer: examples/scoreServer.cpp libs
	$(CC) examples/scoreServer.cpp src/libbltl.a src/libmining.a $(NVCCFLAGS) -o scoreServer
libs:
	make -C src


clean:
	$(MAKE) -C src clean
	rm -f fileModel gpuModel modelChecker gpuChecker
