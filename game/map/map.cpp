
#include "map.hpp"

map::map(){
  this->width = 16;
  this->height = 16;
  // map genorated here
  board = (tile*) malloc(sizeof(tile*) * (this->width * this->height)); // make a width*height 2d array

  // just set map
  for(int y = 0; y < height; y++){
    for (int x = 0; x < width; x++){
      getTile(x, y)->x = x;
      getTile(x, y)->y = y;
      getTile(x, y)->owner = -1;
      getTile(x, y)->myobject = NULL;
    }
  }

  
}

void map::addStar(int x, int y, int power_level){
  
  (board + y*width + x)->myobject = new star(x, y, power_level);
}

tile * map::getTile(int x, int y){
  return (board + y*width + x);
}

void map::setOwner(int x, int y, int newOwner){
  
}

int map::getOwner(int x, int y){
  
}

int map::getSquareCapacity(int x, int y){

}

int map::getSquareType(int x, int y){

}

void map::setObject(int x, int y, object * obj){

}

void map::removeObject(int x, int y){

}

map::~map(){
  free (board);
}