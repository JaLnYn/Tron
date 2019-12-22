
CC=clang++
CFLAGS=-g -Wall -c
ODIR=obj
GAMDIR=../game
CLIDIR=client
LFLAGS=-lcs50


all: tron_c

game.o: $(GAMDIR)/game.cpp
	$(CC) $(CFLAGS) $(GAMDIR)/game.cpp 
map.o: $(GAMDIR)/map.cpp
	$(CC) $(CFLAGS) $(GAMDIR)/map.cpp 
player.o: $(GAMDIR)/player.cpp
	$(CC) $(CFLAGS) $(GAMDIR)/player.cpp 
clientScreenTest.o: clientScreenTest.cpp
	$(CC) $(CFLAGS) clientScreenTest.cpp 

tron_c: game.o map.o player.o clientScreenTest.o
	$(CC) -g map.o player.o game.o clientScreenTest.o -lncurses -o tron_c 

clean: 
	rm -rf *o tron_s tron_c