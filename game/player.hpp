
#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include "map.hpp"

class player{
private:
  
  int team;
  int x, y;
  int ded;
  int dir; // 0 is up, 1 is left, 2 is down, 3 is right

  int * px; // previous locations, the higher the index, the more recent the location.
  int * py;
public:
  player(int startx, int starty, int startdir);

  // to change the direction of the player
  void changeDir(int dir);

  // tick
  void tick();

  // kills this player
  void kill();

  // get the team
  int getTeam();
  int getDir();
  int getDed();

  unsigned char getUserData();
  void loadUserData(unsigned char l, int x, int y);

  // get x and y
  int getX();
  int getY();

  int getPx(int index);
  int getPy(int index);
  

  ~player();
};



#endif