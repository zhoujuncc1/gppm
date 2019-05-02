NVCC=nvcc
CC=g++
NVCCFLAGS=-O3 -g -std=c++11 -Iinclude -lgsl -lgslcblas -lm -lboost_system -lboost_thread
#-lgslcblas
LIBS=src/libbltl.a src/libmining.a
GPU_LIBS=src/libbltl.a src/libmining.a src/libbltl_gpu.a src/libmining_gpu.a

all: fileModel modelChecker scoreServer

gpu: gpuModel gpuChecker

fileModel: examples/fileApplication.cpp $(LIBS)
	$(CC) examples/fileApplication.cpp $(LIBS) $(NVCCFLAGS) -o fileModel
gpuModel: examples/gpuApplication.cpp $(GPU_LIBS)
	$(NVCC) examples/gpuApplication.cpp $(GPU_LIBS) $(NVCCFLAGS) -o gpuModel 
modelChecker: examples/modelChecker.cpp $(LIBS)
	$(CC) examples/modelChecker.cpp $(LIBS) $(NVCCFLAGS) -o modelChecker
gpuChecker: examples/gpuChecker.cpp $(GPU_LIBS)
	$(NVCC) examples/gpuChecker.cpp $(GPU_LIBS) $(NVCCFLAGS) -o gpuChecker

scoreServer: examples/scoreServer.cpp $(LIBS)
	$(CC) examples/scoreServer.cpp $(LIBS) $(NVCCFLAGS) -o scoreServer

src/libbltl.a:
	make -C src libbltl.a

src/libmining.a:
	make -C src libmining.a

src/libbltl_gpu.a:
	make -C src libbltl_gpu.a

src/libmining_gpu.a:
	make -C src libmining_gpu.a

clean:
	$(MAKE) -C src clean
	rm -f fileModel gpuModel modelChecker gpuChecker

