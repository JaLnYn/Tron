#include "star.hpp"

star::star(int x, int y, int pwr_lvl):object(x, y){
  // genorate star
  int split = rand()%pwr_lvl + 1; 
  this->prd = split;
  this->x_crd = this->prd - split;
}

star::~star(){
}
