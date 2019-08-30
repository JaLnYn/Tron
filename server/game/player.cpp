#include "player.hpp"

player::player(int startx, int starty, int startdir){
  this->x = startx;
  this->y = starty;
  this->dir = startdir;
  ded = false;
  team = dir;

  px = (int *) malloc(sizeof(int) * 3);
  py = (int *) malloc(sizeof(int) * 3);

  for (int i = 0; i< 3; i++){
    *(px + i) = -1;
    *(py + i) = -1;
  }
}

void player::changeDir(int dir){
  this->dir = dir;
}

void player::tick(){
  
  for (int i = 0; i < 2; i++){
    px[i] = px[i + 1];
    py[i] = py[i + 1];
  }
  px[2] = x;
  py[2] = y;

  if(!ded){
    if(dir == 0){
      y--;
    }else if(dir == 1){
      x++;
    }else if(dir == 2){
      y++;
    }else if(dir == 3){
      x--;
    }
  }

}

int player::getTeam(){
  return team;
}

int player::getX(){
  return x;
}

int player::getY(){
  return y;
}


int player::getPx(int index){
  return *(px + index);
}

int player::getPy(int index){
  return *(py + index);
}

player::~player(){

}