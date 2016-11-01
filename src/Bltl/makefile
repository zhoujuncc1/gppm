CC=g++
LEX=flex
YACC=bison
SOURCES=$(wildcard *.c) $(wildcard *.cpp)
INCLUDES=$(wildcard *.h) $(wildcard *.hpp) 
OBJECTS=$(SOURCES:.cpp=.o)

all: $(OBJECTS) lexyacc
	$(CC)  $(OBJECTS) 

lexyacc: parser.ypp lexer.l
	$(YACC) -v -d parser.ypp -o parser.tab.cpp
	$(LEX) lexer.l 

obj: lexyacc $(SOURCES) $(INCLUDES)
	@$(CC) $(CFLAGS) $(SOURCES)
    @echo "Compilation complete!"