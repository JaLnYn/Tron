
#include "map.hpp"

map::map(int width, int height){
  this->width = width;
  this->height = height;
  
  board = (int*) malloc(sizeof(int) * (this->width * this->height)); // make a width*height 2d array
  for (int i = 0; i < this->width * this->height; i++){
    *(board + i) = -1;
  }
  
}

int * map::getAddress(int x, int y){
  return (board + y * width + x);
}

int map::getController(int x, int y){
  return *getAddress(x, y);
}

void map::setControl(int x, int y, int c){
  *getAddress(x, y) = c;
}

map::~map(){
  free (board);
}