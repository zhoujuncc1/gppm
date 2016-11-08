CC=g++
LEX=flex
YACC=bison
SOURCES=$(wildcard *.cpp) lex.yy.c parser.tab.cpp
OBJECTS=$(SOURCES:.cpp=.o) 
TARGET_DIR=../../bin/Bltl

all: $(TARGET_DIR) $(OBJECTS)
	@echo "Done"

parser.tab.cpp: parser.ypp
	$(YACC) -v -d parser.ypp -o parser.tab.cpp
	
lex.yy.c:  lexer.l
	$(LEX) lexer.l 

%.o: %.cpp
	$(CC) -c -o $(TARGET_DIR)/$@ $^
	
$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)