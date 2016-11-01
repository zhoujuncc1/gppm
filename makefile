CC=g++
all: parser
parser:
	make -C src/Bltl lexyacc

test_parser: src/