
CC=clang++
CFLAGS=-g -Wall -c
ODIR=obj
SERDIR=server
GAMDIR=game
CLIDIR=client
LFLAGS=-lcs50


all: tron_s tron_c


game.o: $(GAMDIR)/game.cpp
	$(CC) $(CFLAGS) $(GAMDIR)/game.cpp 
map.o: $(GAMDIR)/map.cpp
	$(CC) $(CFLAGS) $(GAMDIR)/map.cpp 
player.o: $(GAMDIR)/player.cpp
	$(CC) $(CFLAGS) $(GAMDIR)/player.cpp 
server.o: $(SERDIR)/server.cpp
	$(CC) $(CFLAGS) $(SERDIR)/server.cpp 
client.o: $(CLIDIR)/client.cpp
	$(CC) $(CFLAGS) $(CLIDIR)/client.cpp 

tron_s: game.o map.o player.o server.o
	$(CC) map.o player.o game.o server.o -o tron_s 
	
tron_c: game.o map.o player.o client.o
	$(CC) map.o player.o game.o client.o -lncurses -o tron_c 

clean: 
	rm -rf *o tron_s tron_c