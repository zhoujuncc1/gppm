NVCC=nvcc
CC=g++
CPFLAGS= -O3 -std=c++11 -I../../include
CPP_SRCS = $(wildcard *.cpp)
CPP_OBJS = $(patsubst %.cpp,%.o,$(CPP_SRCS))
CU_SRCS = $(wildcard *.cu)
CU_OBJS = $(patsubst %.cu,%.o,$(CU_SRCS))

all: parser.o lexer.o $(CPP_OBJS) 
	
gpu: $(CU_OBJS)

clean:
	rm -f *.o parser.cpp lexer.cpp parser.hpp
parser.cpp:
	bison parser.ypp -d -o parser.cpp
lexer.cpp:
	/bin/sh ylwrap lexer.lpp lex.yy.c lexer.cpp -- flex


%.o: %.cpp
	$(CC) $(CPFLAGS)  -c  $<

%.o: %.cu
	$(NVCC) $(CPFLAGS)  -c  $<


