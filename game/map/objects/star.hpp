#ifndef STAR_H
#define STAR_H

#include "object.hpp"

class star : public object{
private:
  int prd; // amount of production availible on this star
  int x_crd; // extra credits availble

public:
  star(int x, int y, int pwr_lvl);
  ~star();
};


#endif