#ifndef GAME_H
#define GAME_H

#include "map.hpp"
#include "player.hpp"

#define BUF_SIZE 64
#define AMT_PLRS 4
#define MAP_WIDTH 64
#define MAP_HEIGHT 64
class game{

private:
  const int WIDTH = MAP_WIDTH;
  const int HEIGHT = MAP_HEIGHT;

  player * plrs[AMT_PLRS];
  map * myMap;

  void storeNumber(int n, char * c);

public:
  game();
  void tick();
  void setDirection(int player, int dir);
  // change string to 
  void storeGame(char * buf);

  
  ~game();
};

#endif

