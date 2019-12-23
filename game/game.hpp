#ifndef GAME_H
#define GAME_H

#include "map.hpp"
#include "player.hpp"

#define AMT_PLRS 4
#define MAP_WIDTH 16
#define MAP_HEIGHT 16
class game{

private:
  const int WIDTH = MAP_WIDTH;
  const int HEIGHT = MAP_HEIGHT;

  player * plrs[AMT_PLRS];
  map * myMap;

  int tickCount = 0;
  void storeNumber(int n, unsigned char * c);

public:
  game();

  //SERVERSIDE
  void tick();
  void setDirection(int player, int dir);
  // change string to 
  void storeGame(unsigned char * buf);

  //CLIENTSIDE
  // get the value of the block at the location
  // 0-3 being the players, 4-7 being their trail.
  int blockVal(int x, int y);
  void loadGame(unsigned char * buf);

  // this function is for testing only set everything to zero
  void resetBoard();

  void printGame();

  ~game();
};

#endif

