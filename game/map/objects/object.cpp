#include "object.hpp"

object::object(int x, int y){
  this->x = x;
  this->y = y;
  this->capacity = 0;
  this->ctrl_by = -1;
  this->amt_units = 0;
}

object::~object(){
}