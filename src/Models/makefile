CC=g++
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o) 
TARGET_DIR=../../bin/Models

all: $(TARGET_DIR) $(OBJECTS)
	@echo "Done"

%.o: %.cpp
	$(CC) -c -o $(TARGET_DIR)/$@ $^
	
$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)