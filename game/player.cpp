#include "player.hpp"

player::player(int startx, int starty, int startdir){
  this->x = startx;
  this->y = starty;
  this->dir = startdir;
  ded = 0;
  team = dir;

  px = (int *) malloc(sizeof(int) * 6);
  py = (int *) malloc(sizeof(int) * 6);

  for (int i = 0; i< 6; i++){
    *(px + i) = startx;
    *(py + i) = starty;
  }
}

void player::changeDir(int dir){
  this->dir = dir;
}

void player::tick(){
  
  for (int i = 0; i < 6; i++){
    px[i] = px[i + 1];
    py[i] = py[i + 1];
  }
  px[2] = x;
  py[2] = y;

  if(!ded){
    if(dir == 0){
      y--;
    }else if(dir == 1){
      x--;
    }else if(dir == 2){
      y++;
    }else if(dir == 3){
      x++;
    }
  }

}

void player::kill(){
  
}

int player::getTeam(){
  return team;
}

char player::getUserData(){
  char c = 0;
  c += getTeam()*2^6;
  c += dir*2^4;
  c += ded;
  return c;
}
void player::loadUserData(char c, int x,int y){
  team = c/(2^6);
  dir = (c - team*(2^6))/(2^4);
  ded = (c - team*(2^6) - dir*(2^4));
  this->x = x;
  this->y = y;
}

int player::getX(){
  return x;
}

int player::getY(){
  return y;
}


// get ith previous x position
int player::getPx(int index){
  return *(px + index);
}
// get ith previous y position
int player::getPy(int index){
  return *(py + index);
}

player::~player(){

}