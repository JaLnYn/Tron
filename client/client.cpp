#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include "../game/game.hpp"


#define FROM_SER_BUF_SIZE 32
#define TO_SER_BUF_SIZE 8
//load the game onto the window
void loadWin(game* g, WINDOW * w){
  for (int x = 0; x < MAP_WIDTH; x++){
    for(int y = 0; y < MAP_HEIGHT; y++){
      mvwprintw(w, y+1,x*2+1,"%c",g->blockVal(x,y)+'0');
    }
  }
  move(0,0);
  wrefresh(w);
}

int main(int argc, char **argv){
  
  //char toServerBuf[TO_SER_BUF_SIZE];
  //char fromServerBuf[FROM_SER_BUF_SIZE];

// first lets set up n curses
  game*g = new game();
  //g->loadGame("");

  int startx=10,starty=10;
  initscr();
  cbreak();
  noecho();

  WINDOW * board = newwin(MAP_HEIGHT+2,2*MAP_WIDTH+1, starty, startx);
  refresh();  
  box(board, 0, 0);
  
  

	int c = getch();
  int dir;
  while(c != 'c'){
    // change direc
    if(c == 'a'){
      dir = 1;
    }else if(c == 's'){
      dir = 2;
    }else if(c == 'd'){
      dir = 3;
    }else if(c == 'w'){
      dir = 0;
    }
    g->setDirection(0,dir);
    g->tick();
    loadWin(g,board);

    c = getch();
  }
  
	endwin();

  return 0;
}