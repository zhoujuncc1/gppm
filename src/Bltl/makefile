CC=g++
LEX=flex
LEX_FLAG=-+
YACC=bison
SOURCES=parser.tab.cpp lex.yy.c Bltl.cpp time_variable.cpp test_bltl.cpp
OBJECTS=$(SOURCES:.cpp=.o)

test: $(OBJECTS) lexyacc
	$(CC)  $(OBJECTS) 

lexyacc: parser.ypp lexer.l
	$(YACC) -v -d parser.ypp -o parser.tab.cpp
	$(LEX) lexer.l 
	$(CC) -g $(SOURCES)
