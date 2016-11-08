CC=g++
all: parser
parser:
	make -C src/Bltl lexyacc

