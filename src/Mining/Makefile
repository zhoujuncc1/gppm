CC=g++
NVCC=nvcc
CPFLAGS= -O3 -std=c++11 -I../../include 

CPP_SRCS = $(wildcard *.cpp)
CPP_OBJS = $(patsubst %.cpp,%.o,$(CPP_SRCS))
CU_SRCS = $(wildcard *.cu)
CU_OBJS = $(patsubst %.cu,%.o,$(CU_SRCS))

all: $(CPP_OBJS)  

gpu: $(CU_OBJS)
#	$(CC) -o $@ $^

clean:
	rm -f *.o
%.o: %.cpp
	$(CC) $(CPFLAGS)  -c  $<

%.o: %.cu
	$(NVCC) $(CPFLAGS)  -c  $<


