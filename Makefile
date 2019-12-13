
CC=g++
CFLAGS=-c -Wall
ODIR=obj
SERDIR=server
SERGAMDIR=server/game
CLIDIR=client


all: tron_s tron_c

tron_s: game.o map.o player.o server.o
	$(CC) map.o player.o game.o server.o -o tron_s

game.o:
	$(CC) $(CFLAGS) $(SERGAMDIR)/game.cpp
map.o:
	$(CC) $(CFLAGS) $(SERGAMDIR)/map.cpp
player.o:
	$(CC) $(CFLAGS) $(SERGAMDIR)/player.cpp
server.o:
	$(CC) $(CFLAGS) $(SERDIR)/server.cpp
	
tron_c: 
	echo nothing

clean: 
	rm -rf *o tron_s