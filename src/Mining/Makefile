CC=g++
NVCC=nvcc
CPFLAGS= -g -std=c++11 -I../../include 

CPP_SRCS = $(wildcard *.cpp)
CPP_OBJS = $(patsubst %.cpp,%.o,$(CPP_SRCS))
CU_SRCS = $(wildcard *.cu)
CU_OBJS = $(patsubst %.cu,%.o,$(CU_SRCS))

all: $(CPP_OBJS)
#	$(CC) -o $@ $^

clean:
	rm -f *.o
%.o: %.cpp
	$(NVCC) $(CPFLAGS)  -c  $<

%.o: %.cu
	$(NVCC) $(CPFLAGS)  -c  $<


