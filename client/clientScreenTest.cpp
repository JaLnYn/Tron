#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <chrono> //NEW
#include <errno.h>

#include <ncurses.h>
#include "../game/game.hpp"

#define FROM_SER_BUF_SIZE 32
#define TO_SER_BUF_SIZE 8
//load the game onto the window

// NEW
using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(62ms);

void loadWin(game* g, WINDOW * w){
  
  
  for (int x = 0; x < MAP_WIDTH; x++){
    for(int y = 0; y < MAP_HEIGHT; y++){
      mvwprintw(w, y+1,x*2+1,"%c",g->blockVal(x,y)+'0');
    }
  }
  move(0,0);
  wrefresh(w);
}

void setKey(unsigned char * toServerBuf, int key){

  toServerBuf[0] = key/1000 + '0';
  toServerBuf[1] = (key%1000)/100 + '0';
  toServerBuf[2] = (key%100)/10 +'0';
  toServerBuf[3] = key%10 + '0';
}
void setCmd(unsigned char * toServerBuf, int cmd){
  toServerBuf[4] = (cmd)/100 + '0';
  toServerBuf[5] = (cmd%100)/10 +'0';
  toServerBuf[6] = cmd%10 + '0';
}

void setSendItem(unsigned char * toserv, int key, int cmd, unsigned char final){
  bzero(toserv, 8);
  setKey(toserv, key);
  setCmd(toserv, 0);
  toserv[7] = final + '0';
}

int main(int argc, char **argv){
  
  game * g = new game();



  using clock = std::chrono::high_resolution_clock; //new
  timeout(31);

  
  int startx=10,starty=10;
  initscr();
  noecho();

  WINDOW * board = newwin(MAP_HEIGHT+2,2*MAP_WIDTH+1, starty, startx);

  box(board, 0, 0);
  
  refresh();  

  getch();

  bool quit_game = false;
  unsigned int current_tick = 0;
  double previousTickTime = 0;
  double microseconds = 0;
  double scanTime = 0;
  int input = 0;
  while(!quit_game) {
    auto start = clock::now();

    double timeTillNextTick = (start + timestep - clock::now()).count();
    
    
    while(0 < timeTillNextTick-scanTime-previousTickTime){ 
      auto tickStart = clock::now();
      //scan for input here
      input = getch();
      move(0,0);
      printw("%d",input);
      refresh();
      
      scanTime = (clock::now() - tickStart).count();
      if(scanTime < 0){
        scanTime = 0;
      }
      timeTillNextTick = (start + timestep - clock::now()).count();
    }
    
    auto tickStart = clock::now();
    // parse output //TODO
    // tick here
    
    if(input == 'w'){
      g->setDirection(0, 0);
    }else if (input == 'a'){
      g->setDirection(0, 1);
    }else if (input == 's'){
      g->setDirection(0, 2);
    }else if (input == 'd'){
      g->setDirection(0, 3);
    }
    
    g->tick();
    loadWin(g,board);
    

    previousTickTime = (clock::now() - tickStart).count();
    if(previousTickTime < 0){
      previousTickTime = 0;
    }
    auto x = start + timestep - clock::now();
    microseconds = x.count()/1000.0;
    
    if(microseconds < 0){
      microseconds = 0;
    }
    //printf("tick. previous tick time: %lf, slept for %lf\n", previousTickTime, microseconds);
    usleep(microseconds);
    current_tick++;
  }
  
	endwin();

  return 0;
}
