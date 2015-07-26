CC=g++
DEBUG=-g
CFLAGS=-Wall -c $(DEBUG)
LFLAGS=-Wall $(DEBUG)

BIN=bin

all: main.o rpg

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

rpg: main.o
	$(CC) $(LFLAGS) main.o -o $(BIN)/sfml-app -lsfml-graphics -lsfml-window -lsfml-system
