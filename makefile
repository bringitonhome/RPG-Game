CC=g++
DEBUG=-g
CFLAGS=-Wall -c $(DEBUG)
LFLAGS=-Wall $(DEBUG)

BIN=bin
SFML_DEPS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

all: main.o rpg

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

character.o: character.cpp
	$(CC) $(CFLAGS) character.cpp

rpg: main.o character.o
	$(CC) $(LFLAGS) main.o character.o -o $(BIN)/sfml-app $(SFML_DEPS)
