CC=g++
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o) 
TARGET_DIR=../../bin

SUB_DIRS = Bltl Models
all: $(TARGET_DIR) $(SUB_DIRS) $(OBJECTS)
	@echo "Done"

%.o: %.cpp
	$(CC) -c -o $(TARGET_DIR)/$@ $^
	
$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)

%$(SUB_DIR):
	+make -c $@ all