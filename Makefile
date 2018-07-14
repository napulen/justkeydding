CC=g++

BUILD=build
SRC=src
BIN=bin
INCLUDE=include
MKDIR_P=mkdir -p

CFLAGS=-I$(INCLUDE) --std=c++11

.PHONY: directories clean

all: directories justkeydding

directories: $(BUILD) $(BIN)

$(BUILD):
	$(MKDIR_P) $(BUILD)

$(BIN):
	$(MKDIR_P) $(BIN)

clean:
	rm -R $(BUILD)
	rm -R $(BIN)

justkeydding: $(BUILD)/justkeydding.o
	$(CC) -o $(BIN)/justkeydding $(BUILD)/justkeydding.o

$(BUILD)/justkeydding.o: $(SRC)/justkeydding.cpp
	$(CC) -c -o$(BUILD)/justkeydding.o $(SRC)/justkeydding.cpp $(CFLAGS)
