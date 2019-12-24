#include "player.hpp"

player::player(int startx, int starty, int startdir){
  this->x = startx;
  this->y = starty;
  this->dir = startdir;
  ded = 0;
  team = startdir;

  px = (int *) malloc(sizeof(int) * 6);
  py = (int *) malloc(sizeof(int) * 6);

  for (int i = 0; i < 6; i++){
    *(px + i) = startx;
    *(py + i) = starty;
  }
}

void player::changeDir(int dir){
  this->dir = dir;
}

void player::tick(){
  
  for (int i = 0; i < 5; i++){
    px[i] = px[i + 1];
    py[i] = py[i + 1];
  }
  px[5] = x;
  py[5] = y;
  if(!ded){
    if(dir == 0 && y > 0){
      y--;
    }else if(dir == 1 && x > 0){
      x--;
    }else if(dir == 2 && y < 15){
      y++;
    }else if(dir == 3 && x < 15){
      x++;
    }else{
      ded = 1;
    }
  }
  

}

void player::kill(){
  ded = 1;
}

int player::getTeam(){
  return team;
}

int player::getDir(){
  return dir;
}

int player::getDed(){
  return ded;
}

unsigned char player::getUserData(){
  if(team >= 4 || team < 0){
    perror("get userData team out of bounds");
    exit(0);
  }
  if( dir >=4 || dir < 0){
    perror("get userData dir out of bounds");
    exit(0);
  }
  if(ded >1 || ded < 0){
    perror("get userData ded out of bounds");
    exit(0);
  }
  int c = 0;
  c += this->getTeam()*(64);
  c += this->getDir()*16;
  c += this->getDed();
  
  return c;
}
void player::loadUserData(unsigned char c, int x,int y){
  if(team >= 4 || team < 0 || dir >=4 || dir < 0 || ded >1 || ded < 0){
    perror("load userData out of bounds");
    exit(0);
  }
  team = c/(64);
  dir = (c - team*(64))/(16);
  ded = (c - team*(64) - dir*(16));
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
  free(px);
  free(py);
  
}