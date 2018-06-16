CC=g++

BUILD=build
SRC=src
BIN=bin
INCLUDE=include

CFLAGS=-I$(INCLUDE) --std=c++11

justkeydding: $(BUILD)/justkeydding.o
	$(CC) -o $(BIN)/justkeydding $(BUILD)/justkeydding.o

$(BUILD)/justkeydding.o: $(SRC)/justkeydding.cpp
	$(CC) -c -o$(BUILD)/justkeydding.o $(SRC)/justkeydding.cpp $(CFLAGS)
