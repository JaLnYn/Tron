
#ifndef MAP_H
#define MAP_H

#include <stdlib.h> 
class map{
private:
  int * board;
  int width;
  int height;
  
  // get the address of the tile on a specific square
  int * getAddress(int x, int y);
  

public:
  map(int width, int height);
  int getController(int x, int y);
  void setControl(int x, int y, int c);

  ~map();
};

#endif