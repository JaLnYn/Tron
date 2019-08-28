
#ifndef MAP_H
#define MAP_H
#include "objects/star.hpp"


struct tile
{
  int x, y;
  int owner;
  object * myobject;
};

// add in an stars vector to keep track of the availble stars

class map{
private:
  tile * board;
  int width;
  int height;
  
  void addStar(int x, int y, int pwr_lvl);
  void setObject(int x, int y, object * obj);
  void removeObject(int x, int y);
  tile * getTile(int x, int y);
public:
  map();

  void setOwner(int x, int y, int newOwner);
  int getOwner(int x, int y);
  int getSquareCapacity(int x, int y);
  int getSquareType(int x, int y);

  ~map();
};

#endif