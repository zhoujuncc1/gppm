LEX=flex
LEX_FLAG=-+
YACC=bison
parser: lexer.l parser.ypp Bltl.cpp time_variable.cpp 
	$(YACC) -v -d parser.ypp 
	$(LEX) -+ lexer.l
	$(CC) -ly -ll -c lex.yy.cc parser.tab.cpp Bltl.cpp time_variable.cpp 
	