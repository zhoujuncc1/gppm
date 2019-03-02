NVCC=nvcc
CC=g++
CPPFLAGS= -g  -std=c++11 -Iinclude -lgsl -lm -ll -lfl
fileModel: examples/fileApplication.cpp libs
	$(NVCC) $(CPPFLAGS) -o fileModel examples/fileApplication.cpp src/libblstl.a src/libmining.a 
gpuModel: examples/gpuApplication.cpp libs
	$(NVCC) $(CPPFLAGS) -o gpuModel examples/gpuApplication.cpp src/libblstl.a src/libmining.a 
modelChecker: examples/modelChecker.cpp libs
	$(NVCC) $(CPPFLAGS) -o modelModel examples/modelChecker.cpp src/libblstl.a src/libmining.a 
libs:
	make -C src


clean:
	$(MAKE) -C src clean
	rm -f fileModel

